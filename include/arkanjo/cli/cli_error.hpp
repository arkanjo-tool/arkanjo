#pragma once

#include <iostream>
#include <stdexcept>

/**
 * @brief Base class for CLI-related errors.
 * 
 */
class CLIError : public std::runtime_error {
public:
    explicit CLIError(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @brief An error is thrown when a required option is not provided.
 * 
 * eg: if the --config option is required and is not passed by the user.
 */
class OptionMissingError : public CLIError {
public:
    /**
     * @param option Name of the missing option.
     */
    OptionMissingError(const std::string& option)
        : CLIError("Option missing: " + option) {}
};

/**
 * @brief An error is thrown when an unknown command is passed.
 */
class CommandNotFoundError : public CLIError {
public:
    /**
     * @param cmd Command name that was not recognized.
     */
    CommandNotFoundError(const std::string& cmd)
        : CLIError("Unknown command: " + cmd) {}
};