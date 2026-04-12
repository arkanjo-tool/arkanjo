#pragma once

#include <string>
#include <vector>

#include <tree_sitter/api.h>

class FeatureExtractor {
public:
    std::string extract_features(TSNode root, const std::string& source);
};