#pragma once
#include <arkanjo/cli/parser_options.hpp>

/**
 * @brief Basic interface for all CLI commands.
 * 
 * The ICommand interface defines the minimum structure that a command must 
 * implement to be executed by the command-line system.
 */
class ICommand {
protected:
    std::vector<struct option> options_;
    std::string short_opts_;

    ICommand()
        : options_(), short_opts_("") {}

    ICommand(std::vector<struct option> opts, std::string short_opts)
        : options_(std::move(opts)), short_opts_(std::move(short_opts)) {}

public:
    virtual ~ICommand() = default;

    const std::vector<struct option>& get_options() const { return options_; }
    const std::string& get_short_opts() const { return short_opts_; }

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
};
