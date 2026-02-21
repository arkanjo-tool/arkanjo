#include "preprocessor.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

void Preprocessor::save_current_run_params(const fs::path& path) {
    vector<string> config_content;

    string path_message = PATH_MESSAGE + path.string();

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    string time_message = TIME_MESSAGE + std::ctime(&end_time);

    config_content.push_back(path_message);
    config_content.push_back(time_message);

    Utils::write_file_generic(Config::config()->getBasePath() / CONFIG_PATH, config_content);
}

tuple<string, double, bool> Preprocessor::read_parameters() {
    cout << INITIAL_MESSAGE << '\n';
    string similarity_message;

    cout << PROJECT_PATH_MESSAGE << '\n';
    string path_str;
    cin >> path_str;
    fs::path path(path_str);

    cout << MINIMUM_SIMILARITY_MESSAGE << '\n';
    cin >> similarity_message;
    double similarity = stod(similarity_message);

    bool use_duplication_finder_by_tool = false;

    while (true) {
        cout << MESSAGE_DUPLICATION_FINDER_TYPE_1 << '\n';
        cout << MESSAGE_DUPLICATION_FINDER_TYPE_2 << '\n';
        cout << MESSAGE_DUPLICATION_FINDER_TYPE_3 << '\n';
        int x;
        cin >> x;
        if (x == 1) {
            use_duplication_finder_by_tool = true;
        } else if (x == 2) {
            use_duplication_finder_by_tool = false;
        } else {
            cout << INVALID_CODE_DUPLICATION_FINDER << '\n';
            exit(0);
            continue;
        }
        break;
    }

    return {path, similarity, use_duplication_finder_by_tool};
}

void Preprocessor::preprocess(const fs::path& path, double similarity, bool use_duplication_finder_by_tool) {
    cout << BREAKER_MESSAGE << '\n';

    Config* config = Config::config();
    fs::path base_path = config->getBasePath();

    if (fs::exists(base_path)) {
        fs::remove_all(base_path);
    }

    FunctionBreaker function_breaker(path);

    cout << DUPLICATION_MESSAGE << '\n';

    if (use_duplication_finder_by_tool) {
        DuplicationFinderTool duplicationFinder(base_path, similarity);
        duplicationFinder.execute();
    } else {
        DuplicationFinderDiff duplicationFinder(base_path, similarity);
        duplicationFinder.execute();
    }

    save_current_run_params(path);

    cout << END_MESSAGE << '\n';
}

Preprocessor::Preprocessor(bool force_preprocess) {
    Config* config = Config::config();
    fs::path base_path = config->getBasePath();
    if (force_preprocess || !Utils::does_file_exist(base_path / CONFIG_PATH)) {
        auto [path, similarity, use_duplication_finder_by_tool] = read_parameters();
        preprocess(path, similarity, use_duplication_finder_by_tool);
    }
}

Preprocessor::Preprocessor(bool force_preprocess, const fs::path& path, double similarity) {
    Config* config = Config::config();
    fs::path base_path = config->getBasePath();
    if (force_preprocess || !Utils::does_file_exist(base_path / CONFIG_PATH)) {
        preprocess(path, similarity, true);
    }
}
