#include "function_breaker.hpp"
#include <iostream>
#include <unordered_map>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/parser/tree_sitter_parser.hpp>
#include <arkanjo/base/config.hpp>
#include <arkanjo/base/features/source_feature.hpp>
#include <arkanjo/base/features/ast_feature.hpp>
#include <arkanjo/base/features/metadata_feature.hpp>

std::string FunctionBreaker::extract_extension(const fs::path& file_path) {
    std::string ext = file_path.extension().string();
    if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);
    return ext;
}

fs::path FunctionBreaker::build_output_path(
    const fs::path type_path,
    const fs::path& relative_path,
    const std::string& function_name
) {
    auto& cfg = Config::config();
    fs::path base = cfg.base_path / cfg.name_container / type_path;

    std::string ext;
    if (type_path == cfg.info_path) {
        ext = "json";
    } else {
        ext = extract_extension(relative_path);
    }

    return base / relative_path / (function_name + "." + ext);
}

void FunctionBreaker::write_output(
    const fs::path type_path,
    const fs::path& relative_path,
    const std::string& function_name,
    const std::string& content
) {
    fs::path path = build_output_path(type_path, relative_path, function_name);
    Utils::write_file(path, content);
}

std::string FunctionBreaker::create_info_json(
    int line_declaration, int start_number_line,
    int end_number_line, const fs::path& relative_path,
    const std::string& function_name
) {
    return "{"
        "\"file_name\":\"" + relative_path.string() + "\","
        "\"function_name\":\"" + function_name + "\","
        "\"line_declaration\":" + std::to_string(line_declaration) + ","
        "\"start_number_line\":" + std::to_string(start_number_line) + ","
        "\"end_number_line\":" + std::to_string(end_number_line) +
    "}";
}


void FunctionBreaker::file_breaker(
    const fs::path& file_path, const fs::path& folder_path,
    std::function<void(const FunctionData&)> on_function,
    Granularity granularity,
    int minimum_lines
) {
    if (!fs::exists(file_path)) return;

    fs::path relative_path;
    try {
        relative_path = fs::relative(file_path, folder_path);
    } catch (...) {
        return;
    }

    std::string source_code = Utils::read_file(file_path);

    auto handle_unit = [&](const FunctionData& function) {
        auto source = function.get_feature<SourceFeature>();
        auto metadata = function.get_feature<MetadataFeature>();

        if (metadata->end_number_line - metadata->start_number_line <= minimum_lines) {
            return;
        }

        if (on_function)
            on_function(function);

        auto& cfg = Config::config();

        write_output(cfg.source_path, relative_path, function.function_name, source->code + "\n");
        write_output(cfg.header_path, relative_path, function.function_name, metadata->signature);
        write_output(cfg.info_path, relative_path, function.function_name,
            create_info_json(metadata->line_declaration, metadata->start_number_line, metadata->end_number_line, relative_path, function.function_name));
    };

    if (granularity == Granularity::File) {
        TreeSitterParser::process_file_as_unit(file_path, relative_path, source_code, handle_unit);
    } else {
        TreeSitterParser::process_file(file_path, relative_path, source_code, handle_unit);
    }
}

// TODO: It's possible to add parallelism to this function.
int FunctionBreaker::process(
    const fs::path& folder_path,
    std::function<void(const FunctionData&)> on_function,
    int minimum_lines,
    Granularity granularity
) {
    int size_files = 0;

    for (const auto& dirEntry : fs::recursive_directory_iterator(folder_path)) {
        if (!dirEntry.is_regular_file()) continue;

        auto path = dirEntry.path();
        file_breaker(path, folder_path, on_function, granularity, minimum_lines);
        size_files++;
    }

    return size_files;
}
