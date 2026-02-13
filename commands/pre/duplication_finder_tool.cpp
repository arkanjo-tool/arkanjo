#include "duplication_finder_tool.hpp"

DuplicationFinderTool::DuplicationFinderTool(string base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;
}

void DuplicationFinderTool::execute() {

    string command_tool = "python3 -W ignore third-party/duplicate-code-detection-tool/duplicate_code_detection.py -d ";
    command_tool += base_path;
    command_tool += "/source > ";
    command_tool += base_path;
    command_tool += "/output_tool.txt";

    system(command_tool.c_str());

    cout << SAVING_MESSAGE << '\n';

    Parser parser(base_path + "/output_tool.txt", base_path + "/output_parsed.txt", similarity);
}
