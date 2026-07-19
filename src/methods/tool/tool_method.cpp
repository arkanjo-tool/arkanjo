#include <arkanjo/methods/tool/tool_method.hpp>
#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/base/config/config.hpp>
#include <arkanjo/base/features/source_feature.hpp>

#include <iostream>

using fm = FormatterManager;

ToolMethod::ToolMethod(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;
}

void ToolMethod::execute_by_feature(const fs::path& folder_path, const std::string feature_name) {
    fs::path output_parsed =  base_path / "output_parsed.txt";
    if (!feature_name.empty())
        fs::path output_parsed = base_path / ("output_parsed" + feature_name + ".txt");

    fs::path cwd = fs::current_path();
    fs::current_path(folder_path);

    std::string command_tool = "python3 -W ignore ";
    command_tool += Config::config().third_party_dir.string();
    command_tool += "/duplicate-code-detection-tool/duplicate_code_detection.py";
    command_tool += " --project-root-dir ";
    command_tool += folder_path.string();
    command_tool += " -d ";
    command_tool += folder_path.string();

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

void ToolMethod::on_function(const FunctionData& fd) {
    fs::path base = base_path / source_feature_path;

    auto source = fd.get_feature<SourceFeature>();
    if (!source)
        return;

    fs::path relative(fd.path);
    std::string filename = fd.function_name + relative.extension().string();
    fs::path path = base / relative / filename;
    Utils::write_file(path, source->code + "\n");
}

void ToolMethod::execute() {
    fs::path base = base_path / source_feature_path;

    execute_by_feature(base);
}
