#include "preprocessor_clean.hpp"
#include <arkanjo/base/config.hpp>
#include <arkanjo/cli/cli_error.hpp>
#include <filesystem>
#include <iostream>
#include <string> 

namespace fs = std::filesystem;

bool PreprocessorClean::validate(const ParsedOptions& options) {
    clean_all = options.args.count("all") > 0;
    return true;
}

bool PreprocessorClean::run(const ParsedOptions& options) {
    try {
        fs::path base_path = Config::config().base_path;
        fs::path cache_dir = clean_all ? base_path : base_path / Config::config().name_container;

        if (fs::exists(cache_dir)) {
            fs::remove_all(cache_dir);
            fs::create_directories(base_path);

            if (clean_all) {
                std::cout << "Successfully cleaned all cache directories in: " << cache_dir.string() << "\n";
            } 
            else std::cout << "Successfully cleaned cache directory: " << cache_dir.string() << "\n";
        } 
        else std::cout << "Cache directory does not exist, nothing to clean.\n";

    } catch (const std::exception& e) {
        throw CLIError("Error while cleaning cache: " + std::string(e.what()));
    }
    return true;
}
