/**
 * @file utils.hpp
 * @brief Defines utility functions used across all files
 *
 * This header provides generic utility functions that are not project-specific.
 * Includes file operations, string manipulation, and colored output utilities.
 */

#pragma once
#include <cassert>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Utils {

/**
 * @brief Constant string used as a visual delimiter/separator in prints
 */
const std::string LIMITER_PRINT = "---------------------";

/**
 * @brief Permission flags used when creating directories (rwx for owner)
 */
const int MKDIR_FLAG = 0700;

/**
 * @brief ANSI color codes for light terminal backgrounds
 *
 * Contains escape sequences for:
 * - RESET
 * - RED
 * - GREEN
 * - YELLOW
 * - BLUE
 * - MAGENTA
 * - CYAN
 * - GRAY
 * - WHITE
 * - BRIGHT_YELLOW
 * - BLACK
 */
const std::vector<std::string> COLOR_TOKENS_UTILS_LIGTH = {
    "\033[0m",     // RESET
    "\033[31;2m",  // DARK_RED
    "\033[32;2m",  // DARK_GREEN
    "\033[33;2m",  // DARK_YELLOW
    "\033[34;2m",  // DARK_BLUE
    "\033[35;2m",  // DARK_MAGENTA
    "\033[36;2m",  // DARK_CYAN
    "\033[37;2m",  // DARK_GRAY
    "\033[30;2mm", // BLACK
    "\033[33;2m",  // YELLOW
    "\033[30m",    // BLACK
};

/**
 * @brief ANSI color codes for dark terminal backgrounds
 *
 * Contains escape sequences for:
 * - RESET
 * - DARK_RED
 * - DARK_GREEN
 * - DARK_YELLOW
 * - DARK_BLUE
 * - DARK_MAGENTA
 * - DARK_CYAN
 * - DARK_GRAY
 * - BLACK
 * - YELLOW
 * - WHITE
 */
const std::vector<std::string> COLOR_TOKENS_UTILS_DARK = {
    "\033[0m",  // RESET
    "\033[31m", // RED
    "\033[32m", // GREEN
    "\033[33m", // YELLOW
    "\033[34m", // BLUE
    "\033[35m", // MAGENTA
    "\033[36m", // CYAN
    "\033[37m", // GRAY
    "\033[97m", // WHITE
    "\033[33m", // BRIGHT_YELLOW
    "\033[97m"  // WHITE
};

/**
 * @brief Enumeration of available colors for formatted messages
 */
enum COLOR {
    RESET,         ///< Reset to default color
    RED,           ///< Red color
    GREEN,         ///< Green color
    YELLOW,        ///< Yellow color
    BLUE,          ///< Blue color
    MAGENTA,       ///< Magenta color
    CYAN,          ///< Cyan color
    GRAY,          ///< Gray color
    WHITE,         ///< White color
    BRIGHT_YELLOW, ///< Bright yellow color
    BLACK          ///< Black color
};

/**
 * @brief Ensures that a file stream is successfully opened
 * @param file Reference to the input file stream to check
 * @param file_name Name/path of the file being opened (for error message)
 */
void ensure_file_is_open(std::ifstream& file, const std::string& file_name);

/**
 * @brief Reads a file line by line into a vector of strings
 * @param string_path Path to the file to read
 * @return vector<string> Contents of the file as a vector of strings
 */
std::vector<std::string> read_file_generic(const std::string& string_path);

/**
 * @brief Writes content to a file at specified path
 * @param file_path Path where file should be written
 * @param content Vector of strings to write (each element becomes a line)
 */
void write_file_generic(const std::string& file_path, const std::vector<std::string>& content);

/**
 * @brief Creates all parent directories for a given file path
 * @param file_path The full file path for which to create parent directories
 */
void create_parents_folder_of_file_path(const std::string& file_path);

/**
 * @brief Reads and parses a JSON file, uses nlohmann json library
 * @param string_path Path to the JSON file
 * @return json Parsed JSON content
 */
json read_json(const std::string& string_path);

/**
 * @brief Checks if a file exists at the given path
 * @param file_path Path to check
 * @return bool True if file exists and is accessible, false otherwise
 */
bool does_file_exist(const std::string& file_path);

/**
 * @brief Determines if a path refers to a regular file
 * @param path Path to check
 * @return bool True if path is a regular file, false otherwise
 */
bool is_regular_file(const std::string& path);

/**
 * @brief Formats a message with ANSI color codes
 * @param message Text to colorize
 * @param color Color to apply
 * @return string Colorized message with reset code at the end
 */
std::string format_colored_message(const std::string& message, COLOR color);

/**
 * @brief Checks if a character is considered empty/whitespace
 * @param c Character to check
 * @return bool True if character is space or control character, false otherwise
 */
bool is_empty_char(char c);

/**
 * @brief Checks if a character is special (non-alphanumeric and not underscore)
 * @param c Character to check
 * @return bool True if character is special, false otherwise
 */
bool is_special_char(char c);

/**
 * @brief Splits a string by a delimiter into tokens
 * @param s String to split
 * @param delimiter Character to split on
 * @return vector<string> Vector of tokens (empty tokens are omitted)
 */
std::vector<std::string> split_string(const std::string& s, char delimiter);
}; // namespace Utils
