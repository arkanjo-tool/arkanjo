#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <tree_sitter/api.h>

struct Features {
    std::vector<std::string> ast;
};

class FeatureExtractor {
    static constexpr std::string_view FUNCTION_NODES[] = {
        "function_definition",
        "function_item",
        "closure_expression",
        "method_definition",
        "method_declaration"
    };

    static constexpr std::string_view BLOCK_NODES[] = {
        "block",
        "block_expression",
        "compound_statement"
    };

    static constexpr std::string_view NODES[] = {
        "identifier",
        "field_identifier",
        "type_identifier",
        "number_literal",
        "string_literal"
    };

    void walk(TSNode node, Features& features, const std::string& source);

public:
    static bool is_lexical_node(std::string_view type);
    static bool is_block_node(std::string_view type);
    static bool is_function_node(std::string_view type);

    static std::string get_node_text(TSNode node, const std::string& source);

    std::string extract_features(TSNode root, const std::string& source);
};