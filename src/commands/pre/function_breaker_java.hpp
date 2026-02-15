/**
 * @file function_breaker_java.hpp
 * @brief Java function parsing and extraction
 *
 * Implements functionality to parse Java source files and break them
 * into individual function components with metadata.
 *
 * Defines the flow of parsing a java source file in multiples functions
 */

#pragma once

#include "function_breaker_util.hpp"
#include <arkanjo/utils/utils.hpp>
#include <array>
#include <set>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Java function parser and extractor
 *
 * Analyzes Java source code to identify and extract individual methods,
 * including their signatures, bodies, and location information. Handles
 * Java-specific syntax including class methods and nested structures.
 */
class FunctionBreakerJava {
    const int NUMBER_OF_LINES_BEFORE_FOR_FUNCTION_NAME = 7; ///< Lookback range for method name detection
    const int C_RELEVANT_DEPTH = 0;                         ///< Base bracket depth for scope detection
    const int JAVA_RELEVANT_DEPTH = 1;                      ///< Java-specific bracket depth for methods
    const bool IGNORE_EMPTY_FUNCTIONS = true;               ///< Whether to skip empty method bodies
    const bool ALLOW_STRUCTS = false;                       ///< Whether to process class/struct definitions

    /**
     * @brief Finds all bracket pairs with depth information
     * @param brackets_content Processed bracket content
     * @return set<array<int,3>> Bracket pairs with {start_pos, end_pos, depth}
     */
    set<array<int, 3>> find_start_end_and_depth_of_brackets(vector<string> brackets_content);

    /**
     * @brief Finds bracket pairs at method-relevant depth
     * @param brackets_content Processed bracket content
     * @param depth Target depth to match
     * @return set<pair<int,int>> Bracket pairs with {start_pos, end_pos}
     */
    set<pair<int, int>> find_start_end_of_brackets_of_given_depth(vector<string> brackets_content, int depth);

    /**
     * @brief Finds first open bracket position in string
     * @param s String to search
     * @return int Position of first '{' or -1 if not found
     */
    int find_position_first_open_bracket(string s);

    /**
     * @brief Extracts last token from string
     * @param s String to process
     * @return string Last token (space-separated)
     */
    string extract_last_token_of_string(string s);

    /**
     * @brief Creates Line_content object
     * @param line_number Line number in source
     * @param content Line content
     * @return Line_content Structured line information
     */
    Line_content build_line_code(int line_number, string content);

    /**
     * @brief Gets lines preceding method body
     * @param file_content Full file content
     * @param line_start_body_function Starting line of method body
     * @param pos_bracket Bracket position
     * @return vector<Line_content> Preceding lines of method declaration
     */
    vector<Line_content> get_lines_before_body_function(const vector<string>& file_content, int line_start_body_function, int pos_bracket);

    /**
     * @brief Removes parenthesized content at scope end
     * @param code Lines to process
     * @return vector<Line_content> Cleaned lines
     */
    vector<Line_content> remove_parenteses_at_the_end_of_the_scope(vector<Line_content> code);

    /**
     * @brief Removes content until finding closing parenthesis
     * @param code Lines to process
     * @return vector<Line_content> Trimmed lines
     */
    vector<Line_content> remove_content_until_find_parenteses_at_the_end(vector<Line_content> code);

    /**
     * @brief Removes C-style parameter declarations
     * @param code Lines to process
     * @return vector<Line_content> Cleaned declaration lines
     */
    vector<Line_content> remove_parameters_of_declaration_c(vector<Line_content> code);

    /**
     * @brief Removes Java-style parameter declarations
     * @param code Lines to process
     * @return vector<Line_content> Cleaned declaration lines
     */
    vector<Line_content> remove_parameters_of_declaration_java(vector<Line_content> code);

    /**
     * @brief Removes parameters based on language
     * @param code Lines to process
     * @param programming_language Language variant (C/JAVA)
     * @return vector<Line_content> Cleaned declaration lines
     */
    vector<Line_content> remove_parameters_of_declaration(vector<Line_content> code, PROGRAMMING_LANGUAGE programming_language);

    /**
     * @brief Extracts method name and declaration line
     * @param file_content Full file content
     * @param line_start_body_function Starting line of method body
     * @param programming_language Language variant
     * @return pair<string,int> Method name and declaration line number
     */
    pair<string, int> extract_function_name_and_line_from_declaration(const vector<string>& file_content, int line_start_body_function, PROGRAMMING_LANGUAGE programming_language);

    /**
     * @brief Builds method body content
     * @param start_number_line Start line of method
     * @param end_number_line End line of method
     * @param file_content Full file content
     * @return vector<string> Method body lines
     */
    vector<string> build_function_content(int start_number_line, int end_number_line, const vector<string>& file_content);

    /**
     * @brief Builds method header content
     * @param start_number_line Start line of method
     * @param line_declaration Declaration line number
     * @param relative_path File path relative to codebase
     * @param function_name Method name
     * @param file_content Full file content
     * @return vector<string> Header content lines
     */
    vector<string> build_header_content(int start_number_line, int line_declaration, string relative_path, string function_name, const vector<string>& file_content);

    /**
     * @brief Checks if method body is empty
     * @param start_number_line Start line of method
     * @param end_number_line End line of method
     * @param file_content Full file content
     * @return bool True if method body is empty
     */
    bool is_body_function_empty(int start_number_line, int end_number_line, const vector<string>& file_content);

    /**
     * @brief Processes an individual method
     * @param start_number_line Start line of method
     * @param end_number_line End line of method
     * @param relative_path Relative file path
     * @param file_content Full file content
     * @param programming_language Language variant
     */
    void process_function(int start_number_line, int end_number_line, string relative_path, const vector<string>& file_content, PROGRAMMING_LANGUAGE programming_language);

    /**
     * @brief Constructs full file path from components
     * @param file_path File name/path
     * @param folder_path Containing folder path
     * @return string Full file path
     */
    string file_path_from_folder_path(string file_path, string folder_path);

    /**
     * @brief Main Java file parsing driver
     * @param file_path File to parse
     * @param folder_path Containing folder path
     */
    void file_breaker_java(string file_path, string folder_path);

  public:
    /**
     * @brief Constructs Java function breaker and processes file
     * @param file_path Java source file to parse
     * @param folder_path Containing folder path
     */
    FunctionBreakerJava(string file_path, string folder_path);
};
