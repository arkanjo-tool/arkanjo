#include <arkanjo/parser/tree_sitter_parser.hpp>
#include <iostream>
#include <unordered_map>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/parser/feature_extractor.hpp>

std::unordered_map<std::string, TSLanguage* (*)()> get_language_map();
std::unordered_map<std::string, std::string> get_extension_map();

bool TreeSitterParser::is_function_empty(TSNode body) {
    if (ts_node_is_null(body)) return true;

    uint32_t n = ts_node_named_child_count(body);

    return n == 0;
}

std::string TreeSitterParser::detect_language(const std::string& path) {
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

void TreeSitterParser::collect_functions(
    TSNode node, 
    const std::string& source, 
    const fs::path& relative_path,
    std::function<void(const ParsedFunction&, std::string)> callback
) {
    std::string_view type = ts_node_type(node);
    if (FeatureExtractor::is_function_node(type)) {
        TSPoint start = ts_node_start_point(node);
        TSPoint end = ts_node_end_point(node);

        TSNode body = get_body(node);

        if (is_function_empty(body)) return;
        
        std::string function_name = get_function_name(node, source);
        if (function_name.empty()) return;

        TSPoint body_start = ts_node_start_point(body);
        uint32_t start_byte = ts_node_start_byte(node);
        uint32_t end_byte = ts_node_end_byte(node);

        std::string signature = get_full_signature(node, source);
        std::string code = source.substr(start_byte + signature.size(), end_byte - (start_byte + signature.size()));

        FeatureExtractor extractor;
        auto features = extractor.extract_features(body, source);

        callback({
            function_name, signature, code, start.row, body_start.row, end.row 
        }, features);
    }

    uint32_t count = ts_node_child_count(node);
    for (uint32_t i = 0; i < count; i++) {
        collect_functions(
            ts_node_child(node, i),
            source,
            relative_path,
            callback
        );
    }
}

void TreeSitterParser::process_file(
    const fs::path& file_path,
    const fs::path& relative_path,
    const std::string& source_code,
    std::function<void(const ParsedFunction&, std::string)> callback
) {
    auto lang_name = detect_language(file_path);

    if (lang_name.empty()) return;

    static auto language_map = get_language_map();
    auto it = language_map.find(lang_name);
    if (it == language_map.end()) {
        std::cerr << "Language not found: " << lang_name << "\n";
        return;
    }
    TSLanguage* language = it->second();

    static TSLanguage* current_language = nullptr;
    static std::unique_ptr<TSParser, decltype(&ts_parser_delete)> parser(
        ts_parser_new(),
        ts_parser_delete
    );
    if (current_language != language) {
        if (!ts_parser_set_language(parser.get(), language)) {
            return;
        }
        current_language = language;
    }

    TSTree* tree = ts_parser_parse_string(
        parser.get(),
        nullptr,
        source_code.c_str(),
        source_code.size()
    );
    TSNode root_node = ts_tree_root_node(tree);

    collect_functions(root_node, source_code, relative_path, callback);

    ts_tree_delete(tree);
}
