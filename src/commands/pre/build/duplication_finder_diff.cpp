#include <arkanjo/formatter/format_manager.hpp>
#include "duplication_finder_diff.hpp"
#include <arkanjo/base/config.hpp>
#include <arkanjo/utils/utils.hpp>

using fm = FormatterManager;

DuplicationFinderDiff::DuplicationFinderDiff(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;

    if (similarity < 0) {
        std::cerr << "SIMILARITY SHOULD BE GREATER OR EQUAL 0 TO USE DUPLICATION FINDER BY DIFF COMMAND";
    }
}

std::vector<std::string> DuplicationFinderDiff::find_files(const fs::path& folder_path) {
    std::vector<std::string> file_paths;
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder_path)) {
        fs::path file_path = dirEntry.path();
        if (std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path)) {
            file_paths.push_back(file_path.string());
        }
    }
    return file_paths;
}

bool DuplicationFinderDiff::is_empty_line(std::string line) {
    size_t line_size = line.size();
    bool is_empty = true;
    for (size_t i = 1; i < line_size; i++) {
        is_empty &= Utils::is_empty_char(line[i]);
    }
    return is_empty;
}

std::vector<std::string> DuplicationFinderDiff::remove_blank_lines(std::vector<std::string> content) {
    std::vector<std::string> ret;
    for (auto line : content) {
        if (!is_empty_line(line)) {
            ret.push_back(line);
        }
    }
    return ret;
}

bool DuplicationFinderDiff::is_equal_files(std::vector<std::string> content1, std::vector<std::string> content2) {
    if (content1.size() != content2.size()) {
        return false;
    }
    size_t sz = content1.size();
    for (size_t i = 0; i < sz; i++) {
        if (content1[i] != content2[i]) {
            return false;
        }
    }
    return true;
}

double DuplicationFinderDiff::find_similarity(std::string path1, std::string path2) {
    std::vector<std::string> content1 = Utils::read_file_with_vector(path1);
    std::vector<std::string> content2 = Utils::read_file_with_vector(path2);

    content1 = remove_blank_lines(content1);
    content2 = remove_blank_lines(content2);

    if (is_equal_files(content1, content2)) {
        return 100;
    }

    // execute diff command to extract the differences
    fs::path output_file = base_path / "diff.txt";

    //-c is to print in the desired format
    // 2>&1 is to also send standard error to the output_file
    std::string command = "diff " + path1 + " " + path2 + " -c > " + output_file.string() + " 2>&1";
    system(command.c_str());

    std::vector<std::string> content = Utils::read_file_with_vector(output_file);

    size_t number_lines = content.size();

    int different_lines = 0;

    for (size_t i = 4; i < number_lines; i++) {
        auto& line = content[i];
        if (is_empty_line(line)) {
            continue;
        }
        if (line[0] == '!' || line[0] == '-' || line[0] == '+') {
            different_lines += 1;
        }
    }

    int equal_lines = (int)content1.size() + (int)content2.size();
    equal_lines -= different_lines;

    if (different_lines + equal_lines == 0) {
        return -1;
    }

    double db_equal_lines = equal_lines;
    double db_different_lines = different_lines;
    double similarity_metric = db_equal_lines / (db_different_lines + db_equal_lines);
    similarity_metric *= 100;
    return similarity_metric;
}

std::vector<std::tuple<double, std::string, std::string>> DuplicationFinderDiff::find_similar_pairs(std::vector<std::string>& file_paths) {
    size_t number_files = file_paths.size();
    std::vector<std::tuple<double, std::string, std::string>> ret;
    for (size_t i = 0; i < number_files; i++) {
        for (size_t j = 0; j < number_files; j++)
            if (i != j) {
                std::string file_path1 = file_paths[i];
                std::string file_path2 = file_paths[j];
                double similarity_metric = find_similarity(file_path1, file_path2);
                if (similarity_metric >= similarity) {
                    ret.push_back({similarity_metric, file_path1, file_path2});
                }
            }
    }
    sort(ret.rbegin(), ret.rend());
    return ret;
}

void DuplicationFinderDiff::save_duplications(std::vector<std::tuple<double, std::string, std::string>>& file_duplication_pairs) {
    std::string output_file_path = base_path / "output_parsed.txt";

    auto fout = std::ofstream(output_file_path);

    fout << file_duplication_pairs.size() << '\n';
    for (const auto& [similarity, path1, path2] : file_duplication_pairs) {
        fout << path1 << ' ' << path2 << ' ';
        fout << std::fixed << std::setprecision(2) << similarity << '\n';
    }

    fout.close();
}

void DuplicationFinderDiff::execute() {
    std::vector<std::string> file_paths = find_files(base_path / Config::config().source_path);

    std::vector<std::tuple<double, std::string, std::string>> file_duplication_pairs = find_similar_pairs(file_paths);

    fm::write(SAVING_MESSAGE);

    save_duplications(file_duplication_pairs);
}
