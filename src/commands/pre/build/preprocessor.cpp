#include "preprocessor.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

#include <arkanjo/formatter/format_manager.hpp>

using fm = FormatterManager;

void Preprocessor::save_current_run_params(const fs::path& path) {
    vector<string> config_content;

    string path_message = PATH_MESSAGE + path.string();

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    string time_message = TIME_MESSAGE + std::ctime(&end_time);

    config_content.push_back(path_message);
    config_content.push_back(time_message);

    Utils::write_file_generic(Config::config().base_path / Config::config().name_container / CONFIG_PATH, config_content);
}

tuple<string, double, bool> Preprocessor::read_parameters() {
    fm::write(INITIAL_MESSAGE);
    string similarity_message;

    fm::write(PROJECT_PATH_MESSAGE);
    string path_str;
    cin >> path_str;
    fs::path path(path_str);

    fm::write(MINIMUM_SIMILARITY_MESSAGE);
    cin >> similarity_message;
    double similarity = stod(similarity_message);

    bool use_duplication_finder_by_tool = false;

    while (true) {
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE_1);
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE_2);
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE_3);
        int x;
        cin >> x;
        if (x == 1) {
            use_duplication_finder_by_tool = true;
        } else if (x == 2) {
            use_duplication_finder_by_tool = false;
        } else {
            std::cerr << INVALID_CODE_DUPLICATION_FINDER << '\n';
            exit(0);
            continue;
        }
        break;
    }

    return {path, similarity, use_duplication_finder_by_tool};
}

void Preprocessor::preprocess(const fs::path& path, double similarity, bool use_duplication_finder_by_tool) {
    fm::write(BREAKER_MESSAGE);

    fs::path base_path = Config::config().base_path / Config::config().name_container;

    if (fs::exists(base_path)) {
        fs::remove_all(base_path);
    }

    FunctionBreaker function_breaker(path);

    fm::write(DUPLICATION_MESSAGE);

    if (use_duplication_finder_by_tool) {
        DuplicationFinderTool duplicationFinder(base_path, similarity);
        duplicationFinder.execute();
    } else {
        DuplicationFinderDiff duplicationFinder(base_path, similarity);
        duplicationFinder.execute();
    }

    save_current_run_params(path);

    fm::write(END_MESSAGE);
}

Preprocessor::Preprocessor() { }

Preprocessor::Preprocessor(bool force_preprocess) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    if (force_preprocess || !std::filesystem::exists(base_path / CONFIG_PATH)) {
        auto [path, similarity, use_duplication_finder_by_tool] = read_parameters();
        preprocess(path, similarity, use_duplication_finder_by_tool);
    }
}

Preprocessor::Preprocessor(bool force_preprocess, const fs::path& path, double similarity) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    if (force_preprocess || !std::filesystem::exists(base_path / CONFIG_PATH)) {
        preprocess(path, similarity, true);
    }
}

bool Preprocessor::validate(const ParsedOptions& options) {
    auto it_name = options.args.find("name");
    if (it_name != options.args.end()) {
        Config::config().name_container = it_name->second;
    }
    auto it_json = options.args.find("json");
    if (it_json != options.args.end()) {
        throw CLIError("--json is not supported in this command.");
        return false;
    }
    return true;
}

bool Preprocessor::run([[maybe_unused]] const ParsedOptions& options) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    auto [path, similarity, use_duplication_finder_by_tool] = read_parameters();
    preprocess(path, similarity, use_duplication_finder_by_tool);

    return true;
}