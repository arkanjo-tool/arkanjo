#pragma once

#include <string>
#include <filesystem>
#include <functional>

#include <tree_sitter/api.h>

#include <arkanjo/parser/feature_extractor.hpp>
#include <arkanjo/base/function/function_data.hpp>

namespace fs = std::filesystem;

struct SkipStats {
  size_t errors;
};

class TreeSitterParser {
    static bool is_function_empty(TSNode body);
    
    static std::string detect_language(const fs::path& path);

    static std::string get_full_signature(TSNode func_node, const std::string& source);

    // Extracts the function name from a Tree-sitter function node across multiple languages.
    //
    // Supports different AST structures depending on the language:
    // - C/C++: uses "declarator" field (function_definition)
    // - Rust: uses "name" field (function_item)
    // - Fallback: recursively searches for an identifier-like node
    //
    // This abstraction allows the same FunctionBreaker pipeline to work across
    // multiple Tree-sitter grammars without hardcoding per-language parsers.
    //
    static std::string get_function_name(TSNode func_node, const std::string& source);

    static TSNode get_body(TSNode node);

    /**
     * Returns whether a Tree-sitter subtree should be ignored due to parser errors.
     *
     * The decision is based on the number and density of ERROR nodes in the
     * subtree, helping discard regions where the parser has likely lost
     * synchronization.
     *
     * @param node Root of the subtree to inspect.
     * @return True if the subtree should be ignored.
     */
    static bool should_ignore(TSNode node);

    static void collect_functions(
        TSNode node, const std::string& source, const fs::path& relative_path,
        const std::shared_ptr<TSTree>& tree,
        std::function<void(const FunctionData&)> callback,
        SkipStats& stats);

    static std::shared_ptr<TSTree> parse_source(
        const fs::path& file_path, const std::string& source_code);

  public:
    static void process_file(
      const fs::path& file_path, const fs::path& relative_path, const std::string& source_code,
      std::function<void(const FunctionData&)> callback,
      SkipStats& stats);

    static void process_file_as_unit(
      const fs::path& file_path, const fs::path& relative_path, const std::string& source_code,
      std::function<void(const FunctionData&)> callback,
      SkipStats& stats);

    explicit TreeSitterParser() = default;
};
