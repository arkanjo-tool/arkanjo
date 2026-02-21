#include "function_breaker_util.hpp"

string extract_extension(const fs::path& file_path) {
    std::string ext = file_path.extension().string();
    if (!ext.empty() && ext[0] == '.') ext.erase(0, 1);
    return ext;
}

fs::path build_source_path(const fs::path& relative_path, const string& function_name) {
    string extension = extract_extension(relative_path);
    Config* config = Config::config();
    fs::path final_path = config->getBasePath();
    final_path /= SOURCE_PATH;
    final_path /= relative_path;
    final_path /= function_name + "." + extension;
    return final_path;
}

fs::path build_header_path(const fs::path& relative_path, const string& function_name) {
    string extension = extract_extension(relative_path);
    Config* config = Config::config();
    fs::path final_path = config->getBasePath();
    final_path /= HEADER_PATH;
    final_path /= relative_path;
    final_path /= function_name + "." + extension;
    return final_path;
}

fs::path build_info_path(const fs::path& relative_path, const string& function_name) {
    string extension = extract_extension(relative_path);
    Config* config = Config::config();
    fs::path final_path = config->getBasePath();
    final_path /= INFO_PATH;
    final_path /= relative_path;
    final_path /= function_name + ".json";
    return final_path;
}

void create_source_file(int start_number_line, int end_number_line, const fs::path& relative_path, const string& function_name, const vector<string>& function_content) {
    fs::path path = build_source_path(relative_path, function_name);
    Utils::write_file_generic(path.string(), function_content);
}

void create_header_file(const fs::path& relative_path, const string& function_name, const vector<string>& header_content) {
    fs::path path = build_header_path(relative_path, function_name);
    Utils::write_file_generic(path.string(), header_content);
}

/*This creates a json file*/
void create_info_file(int line_declaration, int start_number_line, int end_number_line, const fs::path& relative_path, const string& function_name) {
    vector<string> content;
    content.push_back("{\n");
    content.push_back("\"file_name\":\"" + relative_path.string() + "\",\n");
    content.push_back("\"function_name\":\"" + function_name + "\",\n");
    content.push_back("\"line_declaration\":" + to_string(line_declaration) + ",\n");
    content.push_back("\"start_number_line\":" + to_string(start_number_line) + ",\n");
    content.push_back("\"end_number_line\":" + to_string(end_number_line) + "\n");
    content.push_back("}\n");
    fs::path path = build_info_path(relative_path, function_name);
    Utils::write_file_generic(path.string(), content);
}
