#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>

#include <tree_sitter/api.h>

/**
 * @brief Extracted AST feature sequences.
 */
struct Features {
    std::vector<std::string> ast; ///< AST node type sequence tokens.
};

/**
 * @brief Utility for classifying Tree-sitter AST nodes and extracting node text.
 */
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

public:
    /**
     * @brief Determines whether a node type is a recognized lexical token node.
     * @param type AST node type string.
     * @return True if lexical token, false otherwise.
     */
    static bool is_lexical_node(std::string_view type);

    /**
     * @brief Determines whether a node type represents a block or compound statement.
     * @param type AST node type string.
     * @return True if block node, false otherwise.
     */
    static bool is_block_node(std::string_view type);

    /**
     * @brief Determines whether a node type represents a function or method definition.
     * @param type AST node type string.
     * @return True if function node, false otherwise.
     */
    static bool is_function_node(std::string_view type);

    /**
     * @brief Extracts the raw source text corresponding to a Tree-sitter AST node.
     * @param node Tree-sitter AST node.
     * @param source Full source code buffer.
     * @return Extracted substring.
     */
    static std::string get_node_text(TSNode node, const std::string& source);
};