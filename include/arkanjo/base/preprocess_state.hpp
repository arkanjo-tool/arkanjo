#pragma once

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

using json = nlohmann::json;

struct PreprocessRunParams {
    fs::path path;             ///< Path of the current preprocess
    std::string finished_time; ///< Timing information for when preprocessing finished
    std::string version;       ///< Version of the current preprocess 
    std::uintmax_t size;       ///< Size of the preprocess output
};

inline void to_json(json& j, const PreprocessRunParams& d) {
    j = {
        {"path", d.path.string()},
        {"finished_time", d.finished_time},
        {"version", d.version},
        {"size", d.size},
    };
}

inline void from_json(const json& j, PreprocessRunParams& d) {
    d.path = j.value("path", "");
    d.finished_time = j.value("finished_time", "");
    d.version = j.value("version","");
    d.size = j.value("size", 0);
}

class Preprocess_State {
protected:
    static constexpr const char* CONFIG_PATH = "config.json"; ///< Configuration file path

public:
    /**
     * @brief Saves preprocessing parameters for future runs
     * @param path Project path to save
     * @param cache_path Project cache path
     */
    static void save_current_run_params(const fs::path& path, const fs::path& cache_path);

    /**
     * @brief read preprocessing parameters runs
     */
    static PreprocessRunParams read_current_run_params();

    static bool is_cache_compatible(const std::string& cache_version);
};
