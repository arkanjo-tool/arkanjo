#include <arkanjo/methods/llm/llm_method.hpp>
#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/base/config.hpp>
#include <arkanjo/base/features/source_feature.hpp>
#include <arkanjo/utils/utils.hpp>

#include <algorithm>
#include <array>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>

using fm = FormatterManager;

LLMMethod::LLMMethod(const fs::path& base_path_, double similarity_,
                     std::vector<std::string> pass_through_args_) {
    base_path = base_path_;
    similarity = similarity_;
    pass_through_args = std::move(pass_through_args_);

    if (similarity < 0) {
        std::cerr << "SIMILARITY SHOULD BE GREATER OR EQUAL 0 TO USE DUPLICATION FINDER BY LLM EMBEDDINGS";
    }
}

std::string LLMMethod::build_command(const fs::path& source_dir) const {
    std::string command = "python3 -W ignore ";
    command += Config::config().third_party_dir.string();
    command += "/llm-detection/llm_detection.py";
    command += " --source-dir ";
    command += source_dir.string();
    command += " --min-similarity ";
    command += std::to_string(similarity);

    for (const auto& arg : pass_through_args) {
        command += " '";
        for (char c : arg) {
            if (c == '\'')
                command += "'\\''";
            else
                command += c;
        }
        command += "'";
    }
    return command;
}

bool LLMMethod::parse_line(const std::string& line, DuplicationEntry& entry) const {
    std::vector<std::string> tokens = Utils::split_string(line, '\t');
    if (tokens.size() != 3) {
        return false;
    }

    try {
        double sim = std::stod(tokens[2]);
        entry = {sim, tokens[0], tokens[1]};
    } catch (const std::exception&) {
        return false;
    }
    return true;
}

std::vector<DuplicationEntry> LLMMethod::read_results(FILE* pipe) const {
    std::vector<DuplicationEntry> pairs;
    std::array<char, 4096> buffer{};
    std::string leftover;

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        leftover += buffer.data();

        size_t newline_pos;
        while ((newline_pos = leftover.find('\n')) != std::string::npos) {
            std::string line = leftover.substr(0, newline_pos);
            leftover.erase(0, newline_pos + 1);

            DuplicationEntry entry;
            if (parse_line(line, entry) && std::get<0>(entry) >= similarity) {
                pairs.push_back(entry);
            }
        }
    }

    sort(pairs.rbegin(), pairs.rend());
    return pairs;
}

void LLMMethod::save_duplications(std::vector<DuplicationEntry>& file_duplication_pairs) {
    std::string output_file_path = base_path / "output_parsed.txt";

    auto fout = std::ofstream(output_file_path);

    fout << file_duplication_pairs.size() << '\n';
    for (const auto& [similarity, path1, path2] : file_duplication_pairs) {
        fout << path1 << ' ' << path2 << ' ';
        fout << std::fixed << std::setprecision(2) << similarity << '\n';
    }

    fout.close();
}

void LLMMethod::on_function(const FunctionData& fd) {
    fs::path base = base_path / source_feature_path;

    auto source = fd.get_feature<SourceFeature>();
    if (!source)
        return;

    fs::path relative(fd.path);
    std::string filename = fd.function_name + relative.extension().string();
    fs::path path = base / relative / filename;
    Utils::write_file(path, source->code + "\n");
}

void LLMMethod::execute() {
    fs::path source_dir = base_path / source_feature_path;

    std::string command = build_command(source_dir);

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing the LLM detection script.\n";
        return;
    }

    std::vector<DuplicationEntry> file_duplication_pairs = read_results(pipe);

    pclose(pipe);

    fm::write(SAVING_MESSAGE);

    save_duplications(file_duplication_pairs);
}
