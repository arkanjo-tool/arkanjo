/**
 * @file path.hpp
 * @brief Logical path abstraction for source code elements.
 *
 * Provides a lightweight abstraction over a function path in the codebase.
 * Handles path manipulation and comparison without accessing the filesystem.
 *
 * This class represents the logical location of a function and does not
 * manage storage or file loading.
 */

#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief Represents a logical path to a function in the codebase.
 *
 * Provides path analysis and manipulation utilities without performing
 * filesystem operations.
 */
class Path {
  private:
    fs::path resource_path; ///< Path

  public:
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
     * @brief Gets the underlying filesystem path.
     *
     * @return Original path represented by this object.
     */
    const fs::path& value() const;

    /**
     * @brief Checks if path is empty
     * @return True if no path components
     */
    bool empty() const;

    /**
     * @brief Gets the directory portion of the path.
     *
     * @return Path containing the parent directory.
     */
    fs::path relative_path() const;

    /**
     * @brief Finds common path components with another path.
     *
     * @param path Path to compare.
     * @return List of common path components.
     */
    std::vector<std::string> common_folders(const Path& path) const;

    /**
     * @brief Path comparison operator
     * @param Other path to compare
     * @return True if this path is ordered first
     */
    bool operator<(const Path& path) const;
};
