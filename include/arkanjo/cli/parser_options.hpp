#pragma once
#include <optional>
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
     *
     * Includes everything after a `--` separator: getopt stops option parsing
     * at `--`, so those tokens are kept unchanged here and can be forwarded
     * directly to a method-specific backend. 
     */
    std::vector<std::string> extra_args;

    /**
     * @brief Checks whether an option exists.
     *
     * @param name Option name.
     * @return true if the option exists, otherwise false.
     */
    bool has(const std::string& name) const {
        return args.find(name) != args.end();
    }

    /**
     * @brief Returns the value of an option.
     *
     * @param name Option name.
     * @return The option value, or std::nullopt if it does not exist.
     */
    std::optional<std::string_view> get(const std::string& name) const {
        auto it = args.find(name);

        if (it == args.end())
            return std::nullopt;

        return it->second;
    }

    /**
     * @brief Returns the value of an option converted to the requested type.
     *
     * @tparam T Target type.
     * @param name Option name.
     * @return The converted value, or std::nullopt if the option does not exist.
     *
     * @throws std::invalid_argument If the value cannot be converted.
     * @throws std::out_of_range If the value is outside the supported range.
     */
    template<typename T> std::optional<T> get_as(const std::string& name) const {
        auto value = get(name);

        if (!value)
            return std::nullopt;

        if constexpr (std::is_same_v<T, std::string>) {
            return std::string(*value);
        } else if constexpr (std::is_same_v<T, int>) {
            return std::stoi(std::string(*value));
        } else if constexpr (std::is_same_v<T, size_t>) {
            return static_cast<size_t>(std::stoull(std::string(*value)));
        } else if constexpr (std::is_same_v<T, double>) {
            return std::stod(std::string(*value));
        } else if constexpr (std::is_same_v<T, bool>) {
            return *value == "true";
        } else {
            static_assert(!sizeof(T), "Unsupported type");
        }
    }
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
 * @param options Array of `CliOption` containing the options.
 * @param ctx_options Reference to ParsedOptions that will be filled in.
 */
bool parse_options(int argc, char* argv[], const std::vector<CliOption>& options, ParsedOptions& ctx_options);
