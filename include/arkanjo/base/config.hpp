/**
 * @file config.hpp
 * @brief Configuration management interface
 *
 * Defines the singleton configuration class that manages application paths.
 * Provides abstraction for file paths in the temporary codebase without
 * requiring direct file access.
 *
 * Contains the abstraction of a path to a function in the temporary code base.
 * Useful for elements that does not need to, in fact, read the files in the
 * temporary codebase.
 */

#pragma once

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#ifndef THIRD_PARTY_DIR
  #define THIRD_PARTY_DIR "third-party"
#endif

/**
 * @brief Singleton configuration manager class
 *
 * Manages application paths and runtime configuration settings.
 * Implements the singleton pattern to ensure single instance access.
 */
class Config {
  protected:
    /**
     * @brief Protected constructor for singleton pattern
     */
    Config() = default;

  public:
    std::string program_name{"arkanjo"}; ///< Name of the program
    fs::path base_path{"tmp/arkanjo"}; ///< Default base path for temporary files
    fs::path third_party_dir{THIRD_PARTY_DIR}; ///< Directory containing third-party dependencies
    
    /**
     * @brief Deleted copy constructor
     */
    Config(const Config&) = delete;

    /**
     * @brief Deleted assignment operator
     */
    Config& operator=(const Config&) = delete;

    /**
     * @brief Gets the singleton configuration instance
     * @return Returns the global configuration instance
     */
    static Config& config();

    /**
     * @brief Sets test configuration paths
     *
     * Configures paths for end-to-end testing environment.
     */
    void setTestConfig();

    /**
     * @brief Sets default production configuration paths
     */
    void setDefaultConfig();
};
