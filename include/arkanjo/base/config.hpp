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

/**
 * @brief Singleton configuration manager class
 *
 * Manages application paths and runtime configuration settings.
 * Implements the singleton pattern to ensure single instance access.
 */
class Config {
    std::string base_path = "tmp"; ///< Default base path for temporary files

  protected:
    static Config* config_; ///< Static instance pointer for singleton pattern

    /**
     * @brief Protected constructor for singleton pattern
     */
    Config() = default;

  public:
    /**
     * @brief Deleted copy constructor
     */
    Config(Config& other) = delete;

    /**
     * @brief Deleted assignment operator
     */
    void operator=(const Config&) = delete;

    /**
     * @brief Gets the singleton configuration instance
     * @return Config* Pointer to the singleton instance
     */
    static Config* config();

    /**
     * @brief Gets the current base path
     * @return string The configured base path
     */
    const std::string& getBasePath() const;

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
