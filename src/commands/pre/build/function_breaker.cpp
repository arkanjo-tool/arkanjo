#include "function_breaker.hpp"
#include <iostream>
#include <unordered_map>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/parser/tree_sitter_parser.hpp>
#include <arkanjo/base/config.hpp>

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


void FunctionBreaker::file_breaker(const fs::path& file_path, const fs::path& folder_path) {
    if (!fs::exists(file_path)) return;

    fs::path relative_path;
    try {
        relative_path = fs::relative(file_path, folder_path);
    } catch (...) {
        return;
    }

    std::string source_code = Utils::read_file(file_path);

    TreeSitterParser::process_file(file_path, relative_path, source_code, [this, &relative_path](const ParsedFunction& fd, std::string tokens) {
        auto& cfg = Config::config();

        write_output(cfg.source_path, relative_path, fd.function_name, fd.code + "\n");
        write_output(cfg.header_path, relative_path, fd.function_name, fd.signature);
        // write_output(cfg.combined_path, relative_path, fd.function_name, tokens);
        write_output(cfg.info_path, relative_path, fd.function_name,
            create_info_json(fd.line_declaration, fd.start_number_line, fd.end_number_line, relative_path, fd.function_name));
    });
}

// TODO: It's possible to add parallelism to this function.
void FunctionBreaker::function_breaker(const fs::path& folder_path) {
    for (const auto& dirEntry : fs::recursive_directory_iterator(folder_path)) {
        if (!dirEntry.is_regular_file()) continue;

        auto path = dirEntry.path();
        file_breaker(path, folder_path);
    }
}

FunctionBreaker::FunctionBreaker(const fs::path& folder_path) {
    function_breaker(folder_path);
}
