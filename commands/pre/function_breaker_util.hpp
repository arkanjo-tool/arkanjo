/**
 * @file function_breaker_util.hpp
 * @brief Function breaker utilities and constants
 *
 * Defines common utilities, structures, and constants used across
 * the function breaking process for supported programming languages.
 * 
 * Defines util functions, struct and constants used across 
 * the function breaker process for the suported languages
 */

#pragma once

#include <string>
#include <vector>

#include <arkanjo/utils/utils.hpp>
#include <arkanjo/base/config.hpp> 

using namespace std;

/**
 * @brief Structure representing a line of source code
 */
struct Line_content {
    int line_number;    ///< Line number in the original file
    string content;     ///< Text content of the line
};

/**
 * @brief Enumeration of supported programming languages
 */
enum PROGRAMMING_LANGUAGE {
    C,      ///< C programming language
    JAVA    ///< Java programming language
};

const string SOURCE_PATH = "source";   ///< Directory name for source files
const string HEADER_PATH = "header";   ///< Directory name for header files
const string INFO_PATH = "info";       ///< Directory name for metadata files

/**
 * @brief Extracts file extension from path
 * @param file_path Path to the file
 * @return string File extension
 */
string extract_extension(string file_path);

/**
 * @brief Builds source file path for a function
 * @param relative_path Relative path of the original file
 * @param function_name Name of the function
 * @return string Full path for the source file
 */
string build_source_path(string relative_path, string function_name);

/**
 * @brief Builds header file path for a function
 * @param relative_path Relative path of the original file
 * @param function_name Name of the function
 * @return string Full path for the header file
 */
string build_header_path(string relative_path, string function_name);

/**
 * @brief Builds info file path for a function
 * @param relative_path Relative path of the original file
 * @param function_name Name of the function
 * @return string Full path for the info file
 */
string build_info_path(string relative_path, string function_name);

/**
 * @brief Creates source file for a function
 * @param start_number_line Starting line number in original file
 * @param end_number_line Ending line number in original file
 * @param relative_path Relative path of the original file
 * @param function_name Name of the function
 * @param function_content Vector of strings containing the function body
 */
void create_source_file(int start_number_line, int end_number_line, 
                       string relative_path, string function_name, 
                       const vector<string> &function_content);

/**
 * @brief Creates header file for a function
 * @param relative_path Relative path of the original file
 * @param function_name Name of the function
 * @param header_content Vector of strings containing the header content
 */
void create_header_file(string relative_path, string function_name, 
                       const vector<string> &header_content);

/**
 * @brief Creates JSON metadata file for a function
 * @param line_declaration Line number where function is declared
 * @param start_number_line Starting line number of function body
 * @param end_number_line Ending line number of function body
 * @param relative_path Relative path of the original file
 * @param function_name Name of the function
 */
void create_info_file(int line_declaration, int start_number_line, 
                     int end_number_line, string relative_path, 
                     string function_name);
