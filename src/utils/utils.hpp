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
#include <iostream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace Utils {

/**
 * @brief Constant string used as a visual delimiter/separator in prints
 */
static constexpr const char* kLimiterPrint = "---------------------";

/**
 * @brief Ensures that a file stream is successfully opened
 * @param file Reference to the input file stream to check
 * @param file_name Name/path of the file being opened (for error message)
 */
template <typename Stream>
void ensure_file_is_open(const Stream& file, const fs::path& file_name);

/**
 * @brief Reads a file into string
 * @param path Path to the file to read
 * @return string Contents of the file as a string
 */
std::string read_file(const fs::path& path);

/**
 * @brief Writes content to a file at specified path
 * @param path Path where file should be written
 * @param content String to write
 */
void write_file(const fs::path& path, const std::string& content);

/**
 * @brief Reads a file line by line into a vector of strings
 * @param path Path to the file to read
 * @return vector<string> Contents of the file as a vector of strings
 */
std::vector<std::string> read_file_with_vector(const fs::path& path);

/**
 * @brief Writes content to a file at specified path
 * @param path Path where file should be written
 * @param content Vector of strings to write (each element becomes a line)
 */
void write_file_with_vector(const fs::path& path, const std::vector<std::string>& content);

/**
 * @brief Reads and parses a JSON file, uses nlohmann json library
 * @param string_path Path to the JSON file
 * @return json Parsed JSON content
 */
json read_json(const fs::path& string_path);

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

std::string to_uppercase(const std::string input);

/**
 * @brief Computes a simple hash of a string
 * @param content Input string to hash
 * @return std::string Hexadecimal hash string (truncated to 12 chars)
 * @note Uses std::hash, not cryptographically secure
 */
std::string hash(const std::string& content);

/**
 * @brief Calculates the total size of all regular files in a folder
 * @param folder Path to the folder
 * @return std::uintmax_t Total size in bytes
 * @note Skips files/folders without permission
 */
std::uintmax_t folder_size(const fs::path& folder);


/**
 * @brief Formats a byte size into a human-readable string
 * @param bytes Size in bytes
 * @return std::string Formatted string with units (B, KB, MB, GB, TB)
 * @note Uses 1024 as the multiplier between units
 */
std::string format_size(std::uintmax_t bytes);

void open_folder(const std::string& path);

const std::string external_command(std::string program_name, std::string command_name);
}; // namespace Utils

struct Wrapped {
    std::string text;
    size_t spaces;
    bool use_first_line;
};

Wrapped wrapped(const std::string& text, size_t spaces = 1, bool use_first_line = true);
std::ostream& operator<<(std::ostream& os, const Wrapped& w);
