#include <arkanjo/parser/feature_extractor.hpp>

std::string FeatureExtractor::extract_features(TSNode root, const std::string& source) {
    const std::string string = ts_node_string(root);
    return string;
}