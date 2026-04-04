#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class Preprocessor {
private:
    static constexpr const char* PATH_MESSAGE = "path of the current preprocess: "; ///< Current processing path display
    static constexpr const char* TIME_MESSAGE = "Finished time: ";                  ///< Timing information prefix

protected:
    static constexpr const char* CONFIG_PATH = "config.txt";                        ///< Configuration file path

public:
    /**
     * @brief Saves preprocessing parameters for future runs
     * @param path Project path to save
     */
    static void save_current_run_params(const fs::path& path);

    /**
     * @brief read preprocessing parameters runs
     */
    static std::vector<std::string> read_current_run_params();
};