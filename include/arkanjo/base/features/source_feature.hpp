#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;
inline fs::path source_feature_path = fs::path("features") / "source";

struct SourceFeature {
    /**
     * @brief Full source code of the function.
     *
     * Contains the entire function definition, including body.
     */
    std::string code;
};
