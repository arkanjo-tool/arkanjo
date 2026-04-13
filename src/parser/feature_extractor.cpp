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

void FeatureExtractor::walk(TSNode node, Features& features, const std::string& source) {
    const std::string type = ts_node_type(node);

    features.ast.push_back(type);

    uint32_t count = ts_node_child_count(node);
    for (uint32_t i = 0; i < count; i++) {
        TSNode child = ts_node_child(node, i);
        walk(child, features, source);
    }
}

std::string FeatureExtractor::extract_features(TSNode root, const std::string& source) {
    Features features;

    walk(root, features, source);

    std::string out = "";
    out += "\n";
    out += "AST: ";
    for (const auto& t : features.ast) {
        out += t + " ";
    }

    return out;
}