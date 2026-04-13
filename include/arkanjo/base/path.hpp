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
#include <filesystem>
namespace fs = std::filesystem;

/**
 * @brief Path manipulation class for tool-specific directory structure
 *
 * Manages paths to source files, headers, and metadata in the temporary codebase.
 * Provides methods for path construction and analysis without file I/O.
 */
class Path {
  private:
    static constexpr const char* BETWEEN_RELATIVE_AND_FUNCTION_NAME = "::"; ///< Separator for path formatting

    static constexpr const char* JSON_EXTENSION = ".json"; ///< Extension for JSON metadata files
    static constexpr const char* BASE_INIT_STRING = "arkanjo"; ///< Base directory name

    fs::path resource_path; ///< Path

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
     * @param path Path to parse
     */
    explicit Path(const fs::path& path);

    /**
     * @brief Builds source file path
     * @return string Path to source file
     */
    fs::path build_source_path() const;

    /**
     * @brief Builds header file path
     * @return string Path to header file
     */
    fs::path build_header_path() const;

    /**
     * @brief Builds metadata file path
     * @return string Path to JSON metadata file
     */
    fs::path build_info_path() const;

    /**
     * @brief Builds relative path portion
     * @return string Relative path
     */
    fs::path build_relative_path() const;

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

    /**
     * @brief Formats path for display
     * @return string Formatted path string
     */
    std::string format_path_message_in_pair() const;
};
