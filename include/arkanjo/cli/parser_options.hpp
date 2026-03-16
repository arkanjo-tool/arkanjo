#pragma once
#include <string>
#include <map>
#include <vector>
#include <getopt.h>

#include <arkanjo/cli/cli_error.hpp>

enum ArgumentsType {
    NoArgument,
    RequiredArgument,
    OptionalArgument,
    PositionalArgument,
};

struct CliOption {
    const char *long_name;
    char short_name;
    ArgumentsType has_arg;
    const char *description {""};
};

/**
 * @brief This marks the end of a long array of options.
 */
#define OPTION_END {nullptr, 0, NoArgument, nullptr}

struct ParsedOptions {
    /**
     * @brief Map from option name to value.
     * 
     * - For flags without arguments, the value is the string `"true"`.
     * - For options with arguments, the value is the argument passed by the user.
     * eg: { "config": "config.json", "verbose": "true" }
     */
    std::map<std::string, std::string> args;

    /**
     * @brief List of extra arguments that are not options.
     */
    std::vector<std::string> extra_args;
};

/**
 * @brief Analyzes command-line arguments and populates ParsedOptions.
 * 
 * It supports short (-c) and long (--config) options.
 * Flags without arguments receive the value `"true"`.
 * Arguments that are not options are added to `extra_args`.
 * 
 * @param argc Number of arguments passed to the application.
 * @param argv Vector of strings containing the arguments.
 * @param long_opts Array of `struct option` containing the long options.
 * @param ctx_options Reference to ParsedOptions that will be filled in.
 */
bool parse_options(int argc, char* argv[], const std::vector<CliOption>& options, ParsedOptions& ctx_options);