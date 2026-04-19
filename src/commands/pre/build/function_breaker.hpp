/**
 * @file function_breaker.hpp
 * @brief Main function extraction interface
 *
 * Provides the primary interface for processing source code directories
 * and extracting functions/structs into individual files in a temporary repository.
 *
 * This file intends to create the process of reading a directory with
 * code in c or java and extract every function/struct of every .c /.java file as a new file
 * in the temporary repository, aka, tmp/
 *
 * Example of expected behaviour:
 * There is a file example.c with functions a and b, will be create two new
 * files: example/a.c and example/b.c
 */

#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <tree_sitter/api.h>

namespace fs = std::filesystem;

/**
 * @brief Main function extraction processor
 *
 * Analyzes source code directories (C/C++/Java) and extracts each function
 * into individual files within a temporary repository. Handles file organization
 * and delegates language-specific parsing to appropriate handlers.
 *
 * @note Current limitations:
 * - May not work correctly with malformed bracket sequences
 * - May fail on brackets within comments
 * - Example behavior: For file example.c with functions a and b,
 *   creates example/a.c and example/b.c in tmp/ directory
 */
class FunctionBreaker {
    /**
     * @brief Extracts file extension from path
     * @param file_path Path to the file
     * @return string File extension
     */
    std::string extract_extension(const fs::path& file_path);

    /**
     * @brief Builds file path for a function
     * @param type Output type
     * @param relative_path Relative path of the original file
     * @param function_name Name of the function
     * @return Full path for the file
     */

    fs::path build_output_path(const fs::path type_path, const fs::path& relative_path, const std::string& function_name);

    /**
     * @brief Creates file for a function
     * @param type Output type
     * @param relative_path Relative path of the original file
     * @param function_name Name of the function
     * @param content Vector of strings containing the content
     */
    void write_output(const fs::path type_path, const fs::path& relative_path, 
        const std::string& function_name, const std::string& content);

    /**
     * @brief Creates JSON metadata file for a function
     * @param line_declaration Line number where function is declared
     * @param start_number_line Starting line number of function body
     * @param end_number_line Ending line number of function body
     * @param relative_path Relative path of the original file
     * @param function_name Name of the function
     */
    std::string create_info_json(int line_declaration, int start_number_line,
      int end_number_line, const fs::path& relative_path,
      const std::string& function_name);

    /**
     * @brief Processes individual source file
     * @param file_path Path to source file
     * @param folder_path Containing directory path
     */
    void file_breaker(const fs::path& file_path, const fs::path& folder_path);

    /**
     * @brief Processes all files in directory
     * @param folder_path Directory path to process
     */
    void function_breaker(const fs::path& folder_path);

  public:
    /**
     * @brief Constructs function breaker and processes directory
     * @param folder_path Directory containing source files to process
     */
    FunctionBreaker(const fs::path& folder_path);
};
