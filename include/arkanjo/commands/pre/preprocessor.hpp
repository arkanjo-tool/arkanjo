#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class Preprocessor {
private:
    static constexpr const char* PATH_KEY = "path";
    static constexpr const char* TIME_KEY = "finished_time";

protected:
    static constexpr const char* CONFIG_PATH = "config.json";

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
