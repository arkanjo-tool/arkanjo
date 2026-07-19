#include "preprocessor_clean.hpp"
#include <arkanjo/base/config/config.hpp>
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
    fs::path base_path = Config::config().base_path;

    if (base_path.filename() != "arkanjo") {
        throw CLIError("Invalid cache directory.");
    }

    try {
        fs::path cache_dir = clean_all ? base_path : base_path / Config::config().name_container;
        cache_dir = fs::weakly_canonical(cache_dir);

        if (fs::exists(cache_dir)) {
            if (clean_all) {
                for (const auto& entry : fs::directory_iterator(base_path))
                    fs::remove_all(entry.path());

                std::cout << "Successfully cleaned all cache directories in: " << cache_dir.string() << "\n";
            } else {
                fs::remove_all(cache_dir);

                std::cout << "Successfully cleaned cache directory: " << cache_dir.string() << "\n";
            }
        } 
        else std::cout << "Cache directory does not exist, nothing to clean.\n";

    } catch (const std::exception& e) {
        throw CLIError("Error while cleaning cache: " + std::string(e.what()));
    }
    return true;
}
