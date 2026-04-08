#include "function_breaker_util.hpp"
#include <arkanjo/base/config.hpp>
#include <arkanjo/utils/utils.hpp>

std::string extract_extension(const fs::path& file_path) {
    std::string ext = file_path.extension().string();
    if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);
    return ext;
}

fs::path build_source_path(const fs::path& relative_path, const std::string& function_name) {
    std::string extension = extract_extension(relative_path);
    fs::path final_path = Config::config().base_path / Config::config().name_container;
    final_path /= Config::config().source_path;
    final_path /= relative_path;
    final_path /= function_name + "." + extension;
    return final_path;
}

fs::path build_header_path(const fs::path& relative_path, const std::string& function_name) {
    std::string extension = extract_extension(relative_path);
    fs::path final_path = Config::config().base_path / Config::config().name_container;
    final_path /= Config::config().header_path;
    final_path /= relative_path;
    final_path /= function_name + "." + extension;
    return final_path;
}

fs::path build_info_path(const fs::path& relative_path, const std::string& function_name) {
    std::string extension = extract_extension(relative_path);
    fs::path final_path = Config::config().base_path / Config::config().name_container;
    final_path /= Config::config().info_path;
    final_path /= relative_path;
    final_path /= function_name + ".json";
    return final_path;
}

void create_source_file(int start_number_line, int end_number_line, const fs::path& relative_path, const std::string& function_name, const std::vector<std::string>& function_content) {
    fs::path path = build_source_path(relative_path, function_name);
    Utils::write_file_generic(path, function_content);
}

void create_header_file(const fs::path& relative_path, const std::string& function_name, const std::vector<std::string>& header_content) {
    fs::path path = build_header_path(relative_path, function_name);
    Utils::write_file_generic(path, header_content);
}

/*This creates a json file*/
void create_info_file(int line_declaration, int start_number_line, int end_number_line, const fs::path& relative_path, const std::string& function_name) {
    std::vector<std::string> content;
    content.push_back("{\n");
    content.push_back("\"file_name\":\"" + relative_path.string() + "\",\n");
    content.push_back("\"function_name\":\"" + function_name + "\",\n");
    content.push_back("\"line_declaration\":" + std::to_string(line_declaration) + ",\n");
    content.push_back("\"start_number_line\":" + std::to_string(start_number_line) + ",\n");
    content.push_back("\"end_number_line\":" + std::to_string(end_number_line) + "\n");
    content.push_back("}\n");
    fs::path path = build_info_path(relative_path, function_name);
    Utils::write_file_generic(path, content);
}
