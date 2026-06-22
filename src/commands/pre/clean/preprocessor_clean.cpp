#include "preprocessor_clean.hpp"
#include <arkanjo/base/config.hpp>
#include <arkanjo/cli/cli_error.hpp>
#include <filesystem>
#include <iostream>
#include <string> 

namespace fs = std::filesystem;

bool PreprocessorClean::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool PreprocessorClean::run([[maybe_unused]] const ParsedOptions& options) {
    try {
        fs::path cache_dir = Config::config().base_path / Config::config().name_container;

        if (fs::exists(cache_dir)) {
            fs::remove_all(cache_dir);
            std::cout << "Successfully cleaned cache directory: " << cache_dir.string() << "\n";
        } else {
            std::cout << "Cache directory does not exist, nothing to clean.\n";
        }
    } catch (const std::exception& e) {
        // Use CLIError to ensure consistent error reporting across the CLI
        throw CLIError("Error while cleaning cache: " + std::string(e.what()));
    }

    return true;
}
