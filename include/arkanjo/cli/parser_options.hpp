#pragma once
#include <string>
#include <map>
#include <vector>
#include <getopt.h>

/**
 * @brief This marks the end of a long array of options.
 */
#define OPTION_END {nullptr, 0, nullptr, 0}

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
 * @param short_opts String containing short options.
 *                   Place a ':' after an option that expects an argument.
 * @param long_opts Array of `struct option` containing the long options.
 * @param ctx_options Reference to ParsedOptions that will be filled in.
 */
void parse_options(int argc, char* argv[], const std::string& short_opts, struct option* long_opts, ParsedOptions& ctx_options);