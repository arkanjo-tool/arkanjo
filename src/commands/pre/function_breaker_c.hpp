/**
 * @file function_breaker_c.hpp
 * @brief C/C++ function parsing and extraction
 *
 * Implements functionality to parse C/C++ source files and break them
 * into individual function components with metadata.
 *
 * Defines the flow of parsing a source cpp file in multiples functions
 */

#pragma once

#include "function_breaker_util.hpp"
#include <arkanjo/utils/utils.hpp>
#include <array>
#include <cassert>
#include <set>
#include <string>
#include <tuple>
#include <vector>

/**
 * @brief C/C++ function parser and extractor
 *
 * Analyzes C/C++ source code to identify and extract individual functions,
 * including their headers, bodies, and location information. Handles
 * complex C syntax including preprocessor directives and comments.
 */
class FunctionBreakerC {
    const int NUMBER_OF_LINES_BEFORE_FOR_FUNCTION_NAME = 7; ///< Lookback range for function name detection
    const int C_RELEVANT_DEPTH = 0;                         ///< Relevant bracket depth for function detection
    const bool IGNORE_EMPTY_FUNCTIONS = true;               ///< Whether to skip empty function bodies
    const bool ALLOW_STRUCTS = false;                       ///< Whether to process struct definitions

    vector<string> file_content;     ///< Original file content
    vector<vector<bool>> mask_valid; ///< Mask for valid code positions

    /**
     * @brief Checks if position contains a preprocessor define
     * @param line Line number
     * @param pos Column position
     * @return bool True if position contains a define
     */
    bool is_define(int line, int pos);

    /**
     * @brief Filters out comments and preprocessor directives
     * @param mask Reference to validity mask to modify
     * @note Only reliable for compilable code
     */
    void filter_mask_commentaries_and_defines(vector<vector<bool>>& mask);

    /**
     * @brief Builds mask of valid code positions
     * @return vector<vector<bool>> 2D mask of valid code
     */
    vector<vector<bool>> build_mask_valid_code();

    /**
     * @brief Finds all bracket pairs with depth information
     * @return set<array<int,5>> Bracket pairs with {start_line, start_col, end_line, end_col, depth}
     */
    set<array<int, 5>> find_start_end_and_depth_of_brackets();

    /**
     * @brief Finds bracket pairs at function-relevant depth
     * @return set<array<int,4>> Bracket pairs with {start_line, start_col, end_line, end_col}
     */
    set<array<int, 4>> find_start_end_of_brackets_of_given_depth();

    /**
     * @brief Extracts function body content
     * @param start_number_line Start line of function
     * @param start_column Start column of function
     * @param end_number_line End line of function
     * @param end_column End column of function
     * @return vector<string> Function body content
     */
    vector<string> build_function_content(int start_number_line, int start_column, int end_number_line, int end_column);

    /**
     * @brief Advances pointer past parenthesized expressions
     * @param line Current line (modified in-place)
     * @param column Current column (modified in-place)
     * @return bool True if moved successfully
     */
    bool move_pointer_until_character_outside_parenteses(int& line, int& column);

    /**
     * @brief Extracts function header information
     * @param start_line Function start line
     * @param start_column Function start column
     * @return tuple<string,int,vector<string>> Function name, declaration line, and header lines
     */
    tuple<string, int, vector<string>> extract_header_related_information(int start_line, int start_column);

    /**
     * @brief Checks if function body is empty
     * @param start_number_line Start line of function
     * @param start_column Start column of function
     * @param end_number_line End line of function
     * @param end_column End column of function
     * @return bool True if function body is empty
     */
    bool is_body_function_empty(int start_number_line, int start_column, int end_number_line, int end_column);

    /**
     * @brief Processes an individual function
     * @param start_number_line Start line of function
     * @param start_column Start column of function
     * @param end_number_line End line of function
     * @param end_column End column of function
     * @param relative_path Relative path of source file
     */
    void process_function(int start_number_line, int start_column, int end_number_line, int end_column, string relative_path);

    /**
     * @brief Constructs full file path from folder path
     * @param file_path File name/path
     * @param folder_path Containing folder path
     * @return string Full file path
     */
    string file_path_from_folder_path(string file_path, string folder_path);

    /**
     * @brief Main file parsing driver
     * @param file_path File to parse
     * @param folder_path Containing folder path
     */
    void file_breaker_c(string file_path, string folder_path);

  public:
    /**
     * @brief Constructs function breaker and processes file
     * @param file_path Source file to parse
     * @param folder_path Containing folder path
     */
    FunctionBreakerC(string file_path, string folder_path);
};
