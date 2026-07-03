#pragma once

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

using json = nlohmann::json;

struct PreprocessRunParams {
    std::string path;          ///< Path of the current preprocess
    std::string finished_time; ///< Timing information for when preprocessing finished
};

inline void to_json(json& j, const PreprocessRunParams& d) {
    j = {
        {"path", d.path},
        {"finished_time", d.finished_time},
    };
}

inline void from_json(const json& j, PreprocessRunParams& d) {
    d.path = j.value("path", "");
    d.finished_time = j.value("finished_time", "");
}

class Preprocessor {
protected:
    static constexpr const char* CONFIG_PATH = "config.json"; ///< Configuration file path

public:
    /**
     * @brief Saves preprocessing parameters for future runs
     * @param path Project path to save
     */
    static void save_current_run_params(const fs::path& path);

    /**
     * @brief read preprocessing parameters runs
     */
    static PreprocessRunParams read_current_run_params();
};
