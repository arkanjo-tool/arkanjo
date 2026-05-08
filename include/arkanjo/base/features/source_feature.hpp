#pragma once

#include <string>

inline fs::path source_feature_path = fs::path("features") / "source";

struct SourceFeature {
    std::string code;
};
