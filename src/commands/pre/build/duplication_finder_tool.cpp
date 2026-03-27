#include "duplication_finder_tool.hpp"

DuplicationFinderTool::DuplicationFinderTool(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;
}

void DuplicationFinderTool::execute() {
    fs::path output_parsed = base_path / "output_parsed.txt";

    std::string command_tool = "python3 -W ignore ";
    command_tool += Config::config().third_party_dir;
    command_tool += "/duplicate-code-detection-tool/duplicate_code_detection.py -d ";
    command_tool += base_path / "source";

    std::cout << SAVING_MESSAGE << '\n';

    FILE* pipe = popen(command_tool.c_str(), "r");
    if (!pipe) {
        cerr << "Error executing the tool.\n";
        return;
    }

    Parser parser(output_parsed, similarity);
    parser.exec_from_stream(pipe);

    pclose(pipe);
}
