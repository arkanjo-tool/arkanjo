#include <arkanjo/parser/feature_extractor.hpp>

bool FeatureExtractor::is_lexical_node(std::string_view type) {
    for (auto& n : NODES)
        if (n == type) return true;
    return false;
}

bool FeatureExtractor::is_block_node(std::string_view type) {
    for (auto& n : BLOCK_NODES)
        if (n == type) return true;
    return false;
}

bool FeatureExtractor::is_function_node(std::string_view type) {
    for (auto& n : FUNCTION_NODES)
        if (n == type) return true;
    return false;
}

std::string FeatureExtractor::get_node_text(TSNode node, const std::string& source) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);

    return source.substr(start, end - start);
}