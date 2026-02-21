#pragma once
#include <arkanjo/cli/parser_options.hpp>

/**
 * @brief Basic interface for all CLI commands.
 *
 * The ICommand interface defines the minimum structure that a command must
 * implement to be executed by the command-line system.
 */
class ICommand {

  public:
    virtual ~ICommand() = default;

    virtual const option* options() const = 0;
    virtual const char* short_opts() const = 0;
    virtual std::string_view description() const = 0;

    /**
     * @brief Validate the arguments already analyzed.
     *
     * Verify that all required options have been selected and that the values
     * ​​provided are valid.
     *
     * @param options The ParsedOptions structure will be populated with the
     * arguments.
     * @return True if the arguments are valid, false otherwise.
     */
    virtual bool validate(const ParsedOptions& options) = 0;

    /**
     * @brief Executes the main logic of the command.
     *
     * You must use the validated arguments to perform the action desired by
     * the command.
     *
     * @param options The ParsedOptions structure will be populated with the
     * arguments.
     * @return True if the execution is successful, false if an error occurs.
     */
    virtual bool run(const ParsedOptions& options) = 0;
    virtual bool do_run(const ParsedOptions& options) = 0;
};
