/**
 * @file function_storage.hpp
 * @brief Provides access to function storage locations.
 *
 * Responsible for resolving the paths where function-related data is stored,
 * such as source code, headers, and metadata files.
 */

#pragma once

#include <filesystem>

#include <arkanjo/base/path.hpp>

namespace fs = std::filesystem;

class FunctionStorage {
  public:
    /**
      * @brief Builds source file path
      * @return string Path to source file
      */
    fs::path build_source_path(const Path& path) const;

    /**
      * @brief Builds header file path
      * @return string Path to header file
      */
    fs::path build_header_path(const Path& path) const;

    /**
      * @brief Builds metadata file path
      * @return string Path to JSON metadata file
      */
    fs::path build_info_path(const Path& path) const;

  private:
    static constexpr const char* JSON_EXTENSION = ".json"; ///< Extension for JSON metadata files
};
