#include "preprocessor_build.hpp"
#include <cassert>
#include <iomanip>
#include <iostream>

#include <arkanjo/base/config.hpp>
#include <arkanjo/formatter/format_manager.hpp>

using fm = FormatterManager;

std::tuple<std::string, double, bool> PreprocessorBuild::read_parameters() {
    fm::write(INITIAL_MESSAGE);
    std::string similarity_message;

    fm::write(PROJECT_PATH_MESSAGE);
    std::string path_str;
    std::cin >> path_str;
    fs::path path(path_str);

    fm::write(MINIMUM_SIMILARITY_MESSAGE);
    std::cin >> similarity_message;
    double similarity = stod(similarity_message);

    bool use_duplication_finder_by_tool = false;

    while (true) {
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE_1);
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE_2);
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE_3);
        int x;
        std::cin >> x;
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

void PreprocessorBuild::preprocess(const fs::path& path, double similarity, bool use_duplication_finder_by_tool) {
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

    Preprocessor::save_current_run_params(path);

    fm::write(END_MESSAGE);
}

PreprocessorBuild::PreprocessorBuild() { }

PreprocessorBuild::PreprocessorBuild(bool force_preprocess) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    if (force_preprocess || !std::filesystem::exists(base_path / CONFIG_PATH)) {
        auto [path, similarity, use_duplication_finder_by_tool] = read_parameters();
        preprocess(path, similarity, use_duplication_finder_by_tool);
    }
}

PreprocessorBuild::PreprocessorBuild(bool force_preprocess, const fs::path& path, double similarity) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    if (force_preprocess || !std::filesystem::exists(base_path / CONFIG_PATH)) {
        preprocess(path, similarity, true);
    }
}

bool PreprocessorBuild::validate(const ParsedOptions& options) {
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

bool PreprocessorBuild::run([[maybe_unused]] const ParsedOptions& options) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    auto [path, similarity, use_duplication_finder_by_tool] = read_parameters();
    preprocess(path, similarity, use_duplication_finder_by_tool);

    return true;
}