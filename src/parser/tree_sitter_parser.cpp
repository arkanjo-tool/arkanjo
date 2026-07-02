#include <arkanjo/parser/tree_sitter_parser.hpp>
#include <iostream>
#include <unordered_map>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/base/features/ast_feature.hpp>
#include <arkanjo/base/features/source_feature.hpp>
#include <arkanjo/base/features/metadata_feature.hpp>

std::unordered_map<std::string, TSLanguage* (*)()> get_language_map();
std::unordered_map<std::string, std::string> get_extension_map();

bool TreeSitterParser::is_function_empty(TSNode body) {
    if (ts_node_is_null(body)) return true;

    uint32_t n = ts_node_named_child_count(body);

    return n == 0;
}

std::string TreeSitterParser::detect_language(const fs::path& path) {
    static auto ext_map = get_extension_map();

    std::string ext = fs::path(path).extension().string();

    auto it = ext_map.find(ext);
    if (it != ext_map.end()) {
        return it->second;
    }

    return "";
}

std::string TreeSitterParser::get_full_signature(TSNode func_node, const std::string& source) {
    uint32_t signature_start = ts_node_start_byte(func_node);

    TSNode body = get_body(func_node);

    uint32_t signature_end = ts_node_start_byte(body);

    return source.substr(signature_start, signature_end - signature_start);
}

static const char* field_names[] = {
    "name", "declarator"
};

std::string extract_name_generic(TSNode node, const std::string& source) {
    for (const auto& field_name : field_names) {
        TSNode name = ts_node_child_by_field_name(node, field_name, strlen(field_name));
        if (!ts_node_is_null(name)) {
            std::string_view type = ts_node_type(name);
              if (type == "identifier") {
                return FeatureExtractor::get_node_text(name, source);
            }

            if (type == "qualified_identifier") {
                TSNode qualified_identifier_name = ts_node_child_by_field_name(name, "name", strlen("name"));
                if (!ts_node_is_null(qualified_identifier_name)) {
                    return source.substr(
                        ts_node_start_byte(qualified_identifier_name),
                        ts_node_end_byte(qualified_identifier_name) - ts_node_start_byte(qualified_identifier_name)
                    );
                }
            }
        }
    }

    uint32_t count = ts_node_child_count(node);
    for (uint32_t i = 0; i < count; i++) {
        TSNode child = ts_node_child(node, i);
        std::string result = extract_name_generic(child, source);
        if (!result.empty()) return result;
    }

    return "";
}

std::string TreeSitterParser::get_function_name(TSNode func_node, const std::string& source) {
    return extract_name_generic(func_node, source);
}

TSNode TreeSitterParser::get_body(TSNode node) {
    TSNode body = ts_node_child_by_field_name(node, "body", strlen("body"));

    if (!ts_node_is_null(body))
        return body;

    uint32_t count = ts_node_child_count(node);

    for (uint32_t i = 0; i < count; i++) {
        TSNode child = ts_node_child(node, i);
        std::string_view type = ts_node_type(child);
        if (FeatureExtractor::is_block_node(type)) {
            return child;
        }
    }

    return TSNode{};
}

bool TreeSitterParser::should_ignore(TSNode node) {
    size_t errors = 0;
    size_t size = 0;

    std::function<void(TSNode)> dfs = [&](TSNode n) {
        size++;

        if (ts_node_is_error(n))
            errors++;

        uint32_t child_count = ts_node_child_count(n);
        for (uint32_t i = 0; i < child_count; i++)
            dfs(ts_node_child(n, i));
    };

    dfs(node);

    if (size == 0) return false;

    double ratio = static_cast<double>(errors) / static_cast<double>(size);
    int threshold = 1 + static_cast<int>(std::log2(size));

    return (errors > threshold) || (ratio > 0.1);
}

void TreeSitterParser::collect_functions(
    TSNode node, 
    const std::string& source, 
    const fs::path& relative_path,
    const std::shared_ptr<TSTree>& tree,
    std::function<void(const FunctionData&)> callback,
    SkipStats& stats
) {
    std::string_view type = ts_node_type(node);
    if (FeatureExtractor::is_function_node(type)) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);

        if (TreeSitterParser::should_ignore(node)) {
            stats.errors++;
            return;
        }

        TSNode body = get_body(node);

        if (is_function_empty(body)) return;
        
        std::string function_name = get_function_name(node, source);
        if (function_name.empty()) return;

        TSPoint body_start = ts_node_start_point(body);
        uint32_t start_byte = ts_node_start_byte(node);
        uint32_t end_byte = ts_node_end_byte(node);

        std::string signature = get_full_signature(node, source);
        std::string code = source.substr(start_byte + signature.size(), end_byte - (start_byte + signature.size()));

        FunctionData function;
        function.path = relative_path.string();
        function.function_name = function_name;

        auto source = std::make_shared<SourceFeature>();
        source->code = code;
        function.add_feature(source);

        auto ast = std::make_shared<ASTFeature>();
        ast->tree = tree;
        ast->root = body;
        function.add_feature(ast);

        auto metadata = std::make_shared<MetadataFeature>();
        metadata->signature = signature;
        metadata->line_declaration = start.row;
        metadata->start_number_line = body_start.row;
        metadata->end_number_line = end.row;
        function.add_feature(metadata);

        callback(function);
    }

    uint32_t count = ts_node_child_count(node);
    for (uint32_t i = 0; i < count; i++) {
        collect_functions(
            ts_node_child(node, i),
            source,
            relative_path,
            tree,
            callback,
            stats
        );
    }
}

std::shared_ptr<TSTree> TreeSitterParser::parse_source(
    const fs::path& file_path,
    const std::string& source_code
) {
    auto lang_name = detect_language(file_path);

    if (lang_name.empty()) return nullptr;

    static auto language_map = get_language_map();
    auto it = language_map.find(lang_name);
    if (it == language_map.end()) {
        std::cerr << "Language not found: " << lang_name << "\n";
        return nullptr;
    }
    TSLanguage* language = it->second();

    static TSLanguage* current_language = nullptr;
    static std::unique_ptr<TSParser, decltype(&ts_parser_delete)> parser(
        ts_parser_new(),
        ts_parser_delete
    );
    if (current_language != language) {
        if (!ts_parser_set_language(parser.get(), language)) {
            return nullptr;
        }
        current_language = language;
    }

    return std::shared_ptr<TSTree>(
        ts_parser_parse_string(
            parser.get(),
            nullptr,
            source_code.c_str(),
            source_code.size()
        ),
        ts_tree_delete
    );
}

void TreeSitterParser::process_file(
    const fs::path& file_path,
    const fs::path& relative_path,
    const std::string& source_code,
    std::function<void(const FunctionData&)> callback,
    SkipStats& stats
) {
    std::shared_ptr<TSTree> tree = parse_source(file_path, source_code);
    if (!tree) return;

    TSNode root_node = ts_tree_root_node(tree.get());

    collect_functions(root_node, source_code, relative_path, tree, callback, stats);

    tree.reset();
}

void TreeSitterParser::process_file_as_unit(
    const fs::path& file_path,
    const fs::path& relative_path,
    const std::string& source_code,
    std::function<void(const FunctionData&)> callback,
    SkipStats& stats
) {
    std::shared_ptr<TSTree> tree = parse_source(file_path, source_code);
    if (!tree) return;

    TSNode root_node = ts_tree_root_node(tree.get());
    TSPoint end = ts_node_end_point(root_node);

    FunctionData function;
    function.path = relative_path.string();
    function.function_name = relative_path.stem().string();

    auto source = std::make_shared<SourceFeature>();
    source->code = source_code;
    function.add_feature(source);

    auto ast = std::make_shared<ASTFeature>();
    ast->tree = tree;
    ast->root = root_node;
    function.add_feature(ast);

    auto metadata = std::make_shared<MetadataFeature>();
    metadata->signature = "";
    metadata->line_declaration = 0;
    metadata->start_number_line = 0;
    metadata->end_number_line = end.row;
    function.add_feature(metadata);

    callback(function);

    tree.reset();
}
