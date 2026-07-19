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

#include <arkanjo/base/config/schema.hpp>

/**
 * @brief Singleton configuration manager class
 *
 * Manages application paths and runtime configuration settings.
 * Implements the singleton pattern to ensure single instance access.
 */
class Config : public arkanjo::ConfigSchema {
  private:
    /**
     * @brief Validates the current configuration.
     *
     * @throws std::runtime_error If validation fails.
     */
    void validate();

  protected:
    /**
     * @brief Protected constructor for singleton pattern
     */
    Config() = default;

  public:
    
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
