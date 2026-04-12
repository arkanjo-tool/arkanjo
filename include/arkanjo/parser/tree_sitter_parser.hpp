#pragma once

#include <string>
#include <filesystem>
#include <functional>

#include <tree_sitter/api.h>

#include <arkanjo/parser/feature_extractor.hpp>

namespace fs = std::filesystem;

/**
 * @brief Represents a function extracted from source code.
 */
struct ParsedFunction {
  /**
   * @brief Name of the function.
   *
   * Extracted identifier of the function (e.g., "main", "process_data").
   */
  std::string function_name; 

  /**
   * @brief Full function signature.
   *
   * Includes return type, name, parameters, and qualifiers
   * (e.g., "int foo(int a, float b) const").
   */
  std::string signature;

  /**
   * @brief Full source code of the function.
   *
   * Contains the entire function definition, including body.
   */
  std::string code;

  /**
   * @brief Line number where the function is declared.
   *
   * Typically corresponds to the line where the function signature begins.
   */
  size_t line_declaration;

  /**
   * @brief Starting line number of the function body.
   *
   * May differ from declaration line if attributes or macros are present.
   */
  size_t start_number_line;

  /**
   * @brief Ending line number of the function.
   *
   * Corresponds to the last line of the function body.
   */
  size_t end_number_line;
};

class TreeSitterParser {
    static bool is_function_empty(TSNode body);
    
    static std::string detect_language(const std::string& path);

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

    static void collect_functions(
        TSNode node, const std::string& source, const fs::path& relative_path, 
        std::function<void(const ParsedFunction&, std::string)> callback);

  public:
    static void process_file(
      const fs::path& file_path, const fs::path& relative_path, const std::string& source_code, 
      std::function<void(const ParsedFunction&, std::string)> callback);
    
    explicit TreeSitterParser() = default;
};
