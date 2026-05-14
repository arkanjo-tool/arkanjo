#pragma once

#include <filesystem>
#include <tree_sitter/api.h>

namespace fs = std::filesystem;
inline fs::path ast_feature_path = fs::path("features") / "ast";

struct ASTFeature {
    std::shared_ptr<TSTree> tree;
    TSNode root;
};
