#include "preprocessor_build.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <optional>

#include <arkanjo/base/config.hpp>
#include <arkanjo/formatter/format_manager.hpp>

using fm = FormatterManager;

std::tuple<std::string, double, size_t, std::vector<std::string>, Granularity>
PreprocessorBuild::read_parameters(const std::optional<ParsedOptions>& options) {
    fm::write(INITIAL_MESSAGE);
    std::string similarity_message;
    std::string path_str;

    if (options) {
        auto it_path = options->args.find("path");
        if (it_path != options->args.end() && !it_path->second.empty()) {
            path_str = it_path->second;

            if (!fs::exists(path_str)) {
                std::cout << ERROR_PATH_MESSAGE << "\n";
                path_str.clear();
            }
        }
    }

    if (path_str.empty()) {
        fm::write(PROJECT_PATH_MESSAGE);
        std::cin >> path_str;
    }
    fs::path path(path_str);

    double similarity = 0.0;
    if (options) {
        auto it = options->args.find("similarity");
        if (it != options->args.end() && !it->second.empty()) {
            similarity = std::stod(it->second);
        }
    }
    if (similarity == 0.0) {
        fm::write(MINIMUM_SIMILARITY_MESSAGE);
        std::cin >> similarity_message;
        similarity = stod(similarity_message);
    }

    size_t use_duplication_finder_index = 0;
    bool method_from_option = false;

    if (options) {
        auto it = options->args.find("method");
        if (it != options->args.end() && !it->second.empty()) {
            const auto& val = it->second;
            auto found = std::find_if(MethodsType.begin(), MethodsType.end(),
                [&val](const MethodInfo& mi) {
                    return std::to_string(mi.id) == val || mi.name == val;
                });
            if (found == MethodsType.end()) {
                std::string valid_methods;
                for (size_t i = 0; i < MethodsType.size(); ++i) {
                    if (i > 0) valid_methods += ", ";
                    valid_methods += std::to_string(MethodsType[i].id);
                    valid_methods += "|";
                    valid_methods += MethodsType[i].name;
                }
                throw CLIError(
                    "Invalid --method value: '" + val +
                    "'. Use " + valid_methods + ".");
            }
            use_duplication_finder_index = found->id - 1;
            method_from_option = true;
        }
    }

    if (!method_from_option) {
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
    }

    std::vector<std::string> pass_through_args;
    Granularity granularity = Granularity::Function;
    if (options) {
        pass_through_args = options->extra_args;

        auto it = options->args.find("granularity");
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

    return {path, similarity, use_duplication_finder_index, pass_through_args, granularity};
}

void PreprocessorBuild::preprocess(const fs::path& path, double similarity, size_t use_duplication_finder_index,
                                   const std::vector<std::string>& pass_through_args,
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
        base_path, similarity, pass_through_args);

    SkipStats stats = {};

    FunctionBreaker function_breaker;
    auto size_files = function_breaker.process(path, [&method](const FunctionData& fd) {
        method->on_function(fd);
    }, stats, minimum_lines, granularity);

    if (mode_verbose) {
        fm::write("\tFound " + std::to_string(size_files) + " files");
        fm::write("\tSkipped " + std::to_string(stats.errors) +" parser errors");
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
              pass_through_args, granularity] = read_parameters(std::nullopt);
        preprocess(path, similarity, use_duplication_finder_index,
                   pass_through_args, granularity);
    }
}

PreprocessorBuild::PreprocessorBuild(bool force_preprocess, const fs::path& path, double similarity) {
    fs::path base_path = Config::config().base_path / Config::config().name_container;
    if (force_preprocess || !std::filesystem::exists(base_path / CONFIG_PATH)) {
        preprocess(path, similarity, 0);
    }
}

bool PreprocessorBuild::validate(const ParsedOptions& options) {
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
          pass_through_args, granularity] = read_parameters(options);
    preprocess(path, similarity, use_duplication_finder_index,
               pass_through_args, granularity);

    return true;
}