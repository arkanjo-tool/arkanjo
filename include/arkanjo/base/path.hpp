/**
 * @file path.hpp
 * @brief Path abstraction for temporary codebase
 *
 * Provides path manipulation utilities specific to the tool's directory structure.
 * Handles paths without requiring actual file system access.
 * Contains the abstraction of a path to a function in the temporary code base.
 * Useful for elements that does not need to, in fact, read the files in the
 * temporary codebase.
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

/**
 * @brief Path manipulation class for tool-specific directory structure
 *
 * Manages paths to source files, headers, and metadata in the temporary codebase.
 * Provides methods for path construction and analysis without file I/O.
 */
class Path {
  private:
    static constexpr const char* JSON_EXTENSION = ".json"; ///< Extension for JSON metadata files
    static constexpr const char* BASE_INIT_STRING = "tmp"; ///< Base directory name
    static constexpr const char* SOURCE_STRING = "source"; ///< Source subdirectory name
    static constexpr const char* HEADER_STRING = "header"; ///< Header subdirectory name
    static constexpr const char* INFO_STRING = "info";     ///< Info subdirectory name
    static constexpr char BAR = '/';                       ///< Path separator character

    std::vector<std::string> tokens;     ///< Path components
    size_t position_start_relative_path; ///< Index where relative path begins

    /**
     * @brief Splits path string into components
     * @param string_path Path to split
     * @return vector<string> Path components
     */
    std::vector<std::string> split_path(const std::string& string_path);

    /**
     * @brief Finds start position of relative path
     * @return size_t Index in tokens where relative path begins
     */
    size_t find_position_start_relative_path() const;

    /**
     * @brief Joins path components into string
     * @param path Components to join
     * @return string Constructed path
     */
    std::string build_string_path(const std::vector<std::string>& path) const;

    /**
     * @brief Builds path with specified base directory
     * @param base Base directory to use
     * @return string Constructed path
     */
    std::string build_base_path(const std::string& base) const;

    /**
     * @brief Extracts relative path components
     * @return vector<string> Tokens for relative path
     */
    std::vector<std::string> get_tokens_from_relative_path() const;

    /**
     * @brief Removes file extension
     * @param token String with extension
     * @return String without extension
     */
    std::string remove_extension(std::string token) const;

  public:
    /**
     * @brief Checks if path is empty
     * @return bool True if no path components
     */
    bool is_empty() const;

    /**
     * @brief Default constructor
     */
    Path() = default;

    /**
     * @brief Constructs from path string
     * @param string_path Path to parse
     */
    explicit Path(std::string string_path);

    /**
     * @brief Builds source file path
     * @return string Path to source file
     */
    std::string build_source_path() const;

    /**
     * @brief Builds header file path
     * @return string Path to header file
     */
    std::string build_header_path() const;

    /**
     * @brief Builds metadata file path
     * @return string Path to JSON metadata file
     */
    std::string build_info_path() const;

    /**
     * @brief Builds relative path portion
     * @return string Relative path
     */
    std::string build_relative_path() const;

    /**
     * @brief Extracts function name from path
     * @return string Function name without extension
     */
    std::string build_function_name() const;

    /**
     * @brief Finds common folders with another path
     * @param path Other path to compare
     * @return vector<string> Common base folders
     */
    std::vector<std::string> get_common_folders(const Path& path) const;

    /**
     * @brief Checks for pattern in path
     * @param pattern Pattern to search for
     * @return bool True if pattern found
     */
    bool contains_given_pattern(const std::string& pattern) const;

    /**
     * @brief Path comparison operator
     * @param path Other path to compare
     * @return bool True if this path is ordered first
     */
    bool operator<(const Path& path) const;
};
