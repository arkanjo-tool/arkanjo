#pragma once

#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

using json = nlohmann::json;

/**
 * @brief Preprocessing execution summary parameters persisted to cache.
 */
struct PreprocessRunParams {
    fs::path path;             ///< Project path of the current preprocess run.
    std::string finished_time; ///< Timestamp when preprocessing completed.
    std::string version;       ///< Version of the arkanjo preprocessor used.
    std::uintmax_t size;       ///< Total size of the preprocessed files in bytes.
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

/**
 * @brief Manages storage and validation of preprocessing run state.
 */
class Preprocess_State {
protected:
    static constexpr const char* CONFIG_PATH = "config.json"; ///< Configuration file path within cache.

public:
    /**
     * @brief Saves current run parameters to the cache directory.
     * @param path Project root path.
     * @param cache_path Cache storage path.
     */
    static void save_current_run_params(const fs::path& path, const fs::path& cache_path);

    /**
     * @brief Reads preprocessing parameters saved from the previous run.
     * @return PreprocessRunParams with metadata from previous run.
     */
    static PreprocessRunParams read_current_run_params();

    /**
     * @brief Verifies whether the cached preprocessing data is compatible with the current version.
     * @param cache_version Version string recorded in cache.
     * @return True if compatible, false otherwise.
     */
    static bool is_cache_compatible(const std::string& cache_version);
};
