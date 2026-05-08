#include <arkanjo/methods/tool/tool_method.hpp>
#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/base/config.hpp>

#include <iostream>

using fm = FormatterManager;

ToolMethod::ToolMethod(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;
}

void ToolMethod::execute_by_feature(const fs::path& path, const std::string feature_name) {
    fs::path output_parsed =  base_path / "output_parsed.txt";
    if (!feature_name.empty())
        fs::path output_parsed = base_path / ("output_parsed" + feature_name + ".txt");

    fs::path cwd = fs::current_path();
    fs::current_path(base_path / path);

    std::string command_tool = "python3 -W ignore ";
    command_tool += Config::config().third_party_dir;
    command_tool += "/duplicate-code-detection-tool/duplicate_code_detection.py";
    command_tool += " --project-root-dir ";
    command_tool += base_path / path;
    command_tool += " -d ";
    command_tool += base_path / path;

    fm::write(SAVING_MESSAGE);
    
    FILE* pipe = popen(command_tool.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing the tool.\n";
        return;
    }
    
    Parser parser(output_parsed, similarity);
    parser.exec_from_stream(pipe);
    
    fs::current_path(cwd);
    pclose(pipe);
}

void ToolMethod::execute() {
    // execute_by_feature(Config::config().combined_path);
    execute_by_feature(Config::config().source_path);
}
