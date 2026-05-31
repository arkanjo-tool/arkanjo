#include "preprocessor_build.hpp"
#include <cassert>
#include <iostream>
#include <optional>

#include <arkanjo/base/config.hpp>
#include <arkanjo/formatter/format_manager.hpp>

using fm = FormatterManager;

std::tuple<std::string, double, size_t, std::optional<int>, std::optional<int>,
           std::optional<std::string>, Granularity>
PreprocessorBuild::read_parameters(const std::optional<ParsedOptions>& options) {
    fm::write(INITIAL_MESSAGE);
    std::string similarity_message;
    std::string path_str;

    if (options && !options->extra_args.empty() && !options->extra_args[0].empty()) {
        path_str = options->extra_args[0];

        if (!fs::exists(path_str)) {
            std::cout << ERROR_PATH_MESSAGE << "\n";
            path_str.clear();
        }
    }

    if (path_str.empty()) {
        fm::write(PROJECT_PATH_MESSAGE);
        std::cin >> path_str;
    }
    fs::path path(path_str);

    fm::write(MINIMUM_SIMILARITY_MESSAGE);
    std::cin >> similarity_message;
    double similarity = stod(similarity_message);

    size_t use_duplication_finder_index = 0;

    while (true) {
        fm::write(MESSAGE_DUPLICATION_FINDER_TYPE);
        for (size_t i = 0; i < MethodsType.size(); ++i) {
            std::cout << i + 1 << ") " << MethodsType[i].description << '\n';
        }

        std::cin >> use_duplication_finder_index;

        if (
            use_duplication_finder_index == 0 ||
            use_duplication_finder_index > MethodsType.size()
        ) {
            throw CLIError(INVALID_CODE_DUPLICATION_FINDER);
        }
        break;
    }
    --use_duplication_finder_index;

    std::optional<int> llm_max_seq_length;
    std::optional<int> llm_batch_size;
    std::optional<std::string> llm_model;
    Granularity granularity = Granularity::Function;
    if (options) {
        auto it = options->args.find("llm-max-seq-length");
        if (it != options->args.end()) {
            llm_max_seq_length = std::stoi(it->second);
        }
        it = options->args.find("llm-batch-size");
        if (it != options->args.end()) {
            llm_batch_size = std::stoi(it->second);
        }
        it = options->args.find("llm-model");
        if (it != options->args.end()) {
            llm_model = it->second;
        }
        it = options->args.find("granularity");
        if (it != options->args.end()) {
            if (it->second == "file") {
                granularity = Granularity::File;
            } else if (it->second == "function" || it->second.empty()) {
                granularity = Granularity::Function;
            } else {
                throw CLIError("Invalid --granularity value: '" + it->second +
                               "'. Use 'function' (default) or 'file'.");
            }
        }
    }

    return {path, similarity, use_duplication_finder_index, llm_max_seq_length,
            llm_batch_size, llm_model, granularity};
}

void PreprocessorBuild::preprocess(const fs::path& path, double similarity, size_t use_duplication_finder_index,
                                   std::optional<int> llm_max_seq_length,
                                   std::optional<int> llm_batch_size,
                                   std::optional<std::string> llm_model,
                                   Granularity granularity) {
    auto start = std::chrono::high_resolution_clock::now();

    fm::write(BREAKER_MESSAGE);

    fs::path base_path = Config::config().base_path / Config::config().name_container;

    if (fs::exists(base_path)) {
        fs::remove_all(base_path);

        if (mode_verbose)
            fm::write("Removed existing container folder");
    }

    auto start_breaker = std::chrono::high_resolution_clock::now();
    auto method = MethodsType[use_duplication_finder_index].create(
        base_path, similarity, llm_max_seq_length, llm_batch_size, llm_model);

    FunctionBreaker function_breaker;
    auto size_files = function_breaker.process(path, [&method](const FunctionData& fd) {
        method->on_function(fd);
    }, minimum_lines, granularity);

    if (mode_verbose) {
        fm::write("\tFound " + std::to_string(size_files) + " files");
        fm::time("\tExecution time breaker:", start_breaker);
    }

    auto start_duplication = std::chrono::high_resolution_clock::now();
    fm::write(DUPLICATION_MESSAGE);

    method->execute();
    if (mode_verbose)
        fm::time("\tExecution time duplication:", start_duplication);

    Preprocessor::save_current_run_params(path);

    fm::write(END_MESSAGE);

    fm::time("Execution time:", start);
}

PreprocessorBuild::PreprocessorBuild() { }

PreprocessorBuild::PreprocessorBuild(bool force_preprocess) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    if (force_preprocess || !std::filesystem::exists(base_path / CONFIG_PATH)) {
        auto [path, similarity, use_duplication_finder_index,
              llm_max_seq_length, llm_batch_size, llm_model, granularity] = read_parameters(std::nullopt);
        preprocess(path, similarity, use_duplication_finder_index,
                   llm_max_seq_length, llm_batch_size, llm_model, granularity);
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
    auto it_minimum_lines = options.args.find("minimum-lines");
    if (it_minimum_lines != options.args.end()) {
        try {
            minimum_lines = std::stoi(it_minimum_lines->second);
        } catch (const std::invalid_argument&) {
            throw CLIError("--minimum-lines must be a valid number (passing " + it_minimum_lines->second + ")");
            return false;
        } catch (const std::out_of_range&) {
            throw CLIError("--minimum-lines outside the permitted range");
            return false;
        }
    }
    return true;
}

bool PreprocessorBuild::run([[maybe_unused]] const ParsedOptions& options) {
    mode_verbose = options.args.count("verbose") > 0;

    fs::path base_path = Config::config().base_path / Config::config().name_container;
    auto [path, similarity, use_duplication_finder_index,
          llm_max_seq_length, llm_batch_size, llm_model, granularity] = read_parameters(options);
    preprocess(path, similarity, use_duplication_finder_index,
               llm_max_seq_length, llm_batch_size, llm_model, granularity);

    return true;
}