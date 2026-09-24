#pragma once

#include <string>
#include <filesystem>
#include <functional>
#include <memory>

#include <tree_sitter/api.h>

#include <arkanjo/parser/feature_extractor.hpp>
#include <arkanjo/base/function/function_data.hpp>

namespace fs = std::filesystem;

/**
 * @brief Statistics on skipped files or functions due to parsing errors.
 */
struct SkipStats {
  size_t errors; ///< Count of errors encountered during parsing.
};

/**
 * @brief Tree-sitter parser for language detection, AST generation, and function extraction.
 */
class TreeSitterParser {
    static bool is_function_empty(TSNode body);
    
    static std::string detect_language(const fs::path& path);

    static std::string get_full_signature(TSNode func_node, const std::string& source);

    static std::string get_function_name(TSNode func_node, const std::string& source);

    static TSNode get_body(TSNode node);

    /**
     * @brief Returns whether a Tree-sitter subtree should be ignored due to parser errors.
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
    /**
     * @brief Parses a source file into AST, discovers all contained functions, and invokes callback for each.
     * @param file_path Absolute or relative path to the file.
     * @param relative_path Project-relative path for reporting.
     * @param source_code Source file content string.
     * @param callback Callback invoked for each extracted FunctionData.
     * @param stats Output statistics updated with error counts.
     */
    static void process_file(
      const fs::path& file_path, const fs::path& relative_path, const std::string& source_code,
      std::function<void(const FunctionData&)> callback,
      SkipStats& stats);

    /**
     * @brief Parses a source file treating the entire file as a single compilation unit.
     * @param file_path Absolute or relative path to the file.
     * @param relative_path Project-relative path for reporting.
     * @param source_code Source file content string.
     * @param callback Callback invoked for the unit FunctionData.
     * @param stats Output statistics updated with error counts.
     */
    static void process_file_as_unit(
      const fs::path& file_path, const fs::path& relative_path, const std::string& source_code,
      std::function<void(const FunctionData&)> callback,
      SkipStats& stats);

    explicit TreeSitterParser() = default;
};
