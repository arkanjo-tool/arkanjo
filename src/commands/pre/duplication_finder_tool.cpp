#include "duplication_finder_tool.hpp"

DuplicationFinderTool::DuplicationFinderTool(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;
}

void DuplicationFinderTool::execute() {
    fs::path output_tool = base_path / "output_tool.txt";
    fs::path output_parsed = base_path / "output_parsed.txt";

    string command_tool = "python3 -W ignore third-party/duplicate-code-detection-tool/duplicate_code_detection.py -d ";
    command_tool += base_path / "source";
    command_tool += " > ";
    command_tool += output_tool;

    system(command_tool.c_str());

    cout << SAVING_MESSAGE << '\n';

    Parser parser(output_tool, output_parsed, similarity);
}
