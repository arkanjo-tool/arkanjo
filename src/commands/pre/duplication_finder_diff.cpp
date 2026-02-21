#include "duplication_finder_diff.hpp"

DuplicationFinderDiff::DuplicationFinderDiff(const fs::path& base_path_, double similarity_) {
    base_path = base_path_;
    similarity = similarity_;

    if (similarity < 0) {
        std::cout << "SIMILARITY SHOULD BE GREATER OR EQUAL 0 TO USE DUPLICATION FINDER BY DIFF COMMAND";
    }
}

vector<string> DuplicationFinderDiff::find_files(const fs::path& folder_path) {
    vector<string> file_paths;
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(folder_path)) {
        string file_path = dirEntry.path().string();
        if (Utils::does_file_exist(file_path) && Utils::is_regular_file(file_path)) {
            file_paths.push_back(file_path);
        }
    }
    return file_paths;
}

bool DuplicationFinderDiff::is_empty_line(string line) {
    int line_size = line.size();
    bool is_empty = true;
    for (int i = 1; i < line_size; i++) {
        is_empty &= Utils::is_empty_char(line[i]);
    }
    return is_empty;
}

vector<string> DuplicationFinderDiff::remove_blank_lines(vector<string> content) {
    vector<string> ret;
    for (auto line : content) {
        if (!is_empty_line(line)) {
            ret.push_back(line);
        }
    }
    return ret;
}

bool DuplicationFinderDiff::is_equal_files(vector<string> content1, vector<string> content2) {
    if (content1.size() != content2.size()) {
        return false;
    }
    int sz = content1.size();
    for (int i = 0; i < sz; i++) {
        if (content1[i] != content2[i]) {
            return false;
        }
    }
    return true;
}

double DuplicationFinderDiff::find_similarity(string path1, string path2) {
    vector<string> content1 = Utils::read_file_generic(path1);
    vector<string> content2 = Utils::read_file_generic(path2);

    content1 = remove_blank_lines(content1);
    content2 = remove_blank_lines(content2);

    if (is_equal_files(content1, content2)) {
        return 100;
    }

    // execute diff command to extract the differences
    fs::path output_file = base_path / "diff.txt";

    //-c is to print in the desired format
    // 2>&1 is to also send standard error to the output_file
    string command = "diff " + path1 + " " + path2 + " -c > " + output_file.string() + " 2>&1";
    system(command.c_str());

    vector<string> content = Utils::read_file_generic(output_file);

    int number_lines = content.size();

    int different_lines = 0;

    for (int i = 4; i < number_lines; i++) {
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

vector<tuple<double, string, string>> DuplicationFinderDiff::find_similar_pairs(vector<string>& file_paths) {
    size_t number_files = file_paths.size();
    vector<tuple<double, string, string>> ret;
    for (size_t i = 0; i < number_files; i++) {
        for (size_t j = 0; j < number_files; j++)
            if (i != j) {
                string file_path1 = file_paths[i];
                string file_path2 = file_paths[j];
                double similarity_metric = find_similarity(file_path1, file_path2);
                if (similarity_metric >= similarity) {
                    ret.push_back({similarity_metric, file_path1, file_path2});
                }
            }
    }
    sort(ret.rbegin(), ret.rend());
    return ret;
}

void DuplicationFinderDiff::save_duplications(vector<tuple<double, string, string>>& file_duplication_pairs) {
    string output_file_path = base_path / "output_parsed.txt";

    auto fout = ofstream(output_file_path);

    fout << file_duplication_pairs.size() << '\n';
    for (const auto& [similarity, path1, path2] : file_duplication_pairs) {
        fout << path1 << ' ' << path2 << ' ';
        fout << fixed << setprecision(2) << similarity << '\n';
    }

    fout.close();
}

void DuplicationFinderDiff::execute() {

    vector<string> file_paths = find_files(base_path / "source");

    vector<tuple<double, string, string>> file_duplication_pairs = find_similar_pairs(file_paths);

    cout << SAVING_MESSAGE << '\n';

    save_duplications(file_duplication_pairs);
}
