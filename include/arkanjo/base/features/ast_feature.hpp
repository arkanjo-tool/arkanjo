#pragma once

#include <filesystem>
#include <memory>
#include <tree_sitter/api.h>

namespace fs = std::filesystem;
inline fs::path ast_feature_path = fs::path("features") / "ast";

/**
 * @brief Abstract Syntax Tree (AST) feature representation for parsed functions.
 */
struct ASTFeature {
    std::shared_ptr<TSTree> tree; ///< Shared pointer to the Tree-sitter AST.
    TSNode root;                   ///< Root node of the function AST.
};
