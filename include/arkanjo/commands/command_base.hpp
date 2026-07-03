#pragma once

#include <arkanjo/commands/command.hpp>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/base/config.hpp>
#include <algorithm> 

/**
 * @brief Declares the command description override for a command class.
 */
#define COMMAND_DESCRIPTION(str)                       \
    std::string_view description() const override {    \
        static constexpr char description_str[] = str; \
        return description_str;                        \
    }

/**
 * @brief Detects whether a command type declares an `options_` member.
 */
template <typename, typename = std::void_t<>>
struct has_options : std::false_type {};

/**
 * @brief Specialization used when the command type provides CLI options.
 */
template <typename T>
struct has_options<T, std::void_t<decltype(T::options_)>> : std::true_type {};

/**
 * @brief Base implementation shared by CLI commands.
 *
 * The base class provides option discovery, help rendering, and help-aware
 * command execution while leaving command-specific behavior to `Derived::run`.
 */
template <typename Derived>
class CommandBase : public ICommand {

  public:
    /**
     * @brief Prints formatted help text for a command.
     * @param command_name Name or alias used to invoke the command.
     * @param collector Optional collector containing merged global and command options.
     */
    virtual void print_help(const std::string command_name, const OptionsCollector* collector) const {
        constexpr int OPTION_WIDTH = 26;

        std::string s(description());
        std::cout << wrapped(s, 0) << "\n";
        
        const CliOption* command_options = collector == nullptr ? options() : collector->get_options().data();
        if (command_options) {
            std::vector<const CliOption*> vector_arguments, vector_options;
            for (const CliOption* opt = command_options; opt->long_name != nullptr; ++opt) {
                if (opt->has_arg == PositionalArgument) {
                    vector_arguments.push_back(opt);
                } else {
                    vector_options.push_back(opt);
                }
            }

            std::cout << BOLD(UNDERLINE("Usage:")) << " " 
                      << Config::config().program_name << " " 
                      << command_name
                      << (!vector_options.empty() ? " [OPTIONS]" : "") 
                      << (!vector_arguments.empty() ? " [--] [ARGUMENTS]" : "") 
                      << "..." << "\n\n";

            if (!vector_arguments.empty()) {
                std::cout << BOLD(UNDERLINE("Arguments:")) << "\n";

                for (const auto& item : vector_arguments) {
                    std::string opts_str = "  ";
                    opts_str += "<";
                    opts_str += Utils::to_uppercase(item->long_name);
                    opts_str += ">";
                    std::cout << std::left << std::setw(OPTION_WIDTH) << opts_str;
                    if (item->description != nullptr) {
                        std::cout << wrapped(item->description, OPTION_WIDTH);
                    }
                    std::cout << "\n";
                }
            }

            if (!vector_options.empty()) {
                std::sort(vector_options.begin(), vector_options.end(), [](const CliOption* a, const CliOption* b) {
                    return std::string_view(a->long_name) < std::string_view(b->long_name);
                });

                std::cout << BOLD(UNDERLINE("Options:")) << "\n";

                for (const auto& item : vector_options) {
                    std::string opts_str = "  ";
                    if (item->short_name != 0)
                        opts_str += "-" + std::string(1, static_cast<char>(item->short_name));
                    if (!opts_str.empty() && item->long_name && item->short_name)
                        opts_str += ", ";
                    if (!item->short_name) 
                        opts_str += "    ";
                    if (item->long_name)
                        opts_str += "--" + std::string(item->long_name);
                    opts_str = BOLD(opts_str);
                    
                    if (item->has_arg == RequiredArgument || item->has_arg == OptionalArgument) {
                        opts_str += " <" + Utils::to_uppercase(item->long_name) + "> ";
                    }
                    std::cout << std::left << std::setw(OPTION_WIDTH + 8) << opts_str;
                    if (item->description != nullptr) {
                        std::cout << wrapped(item->description, OPTION_WIDTH);
                    }
                    std::cout << "\n";
                }
            }
        }
    }

    /**
     * @brief Returns the command-specific option definitions.
     * @return Pointer to a null-terminated `CliOption` array, or `nullptr` when absent.
     */
    const CliOption* options() const final {
        if constexpr (has_options<Derived>::value) {
            return Derived::options_;
        } else {
            return nullptr;
        }
    }

    /**
     * @brief Runs the command or displays help when requested.
     * @param command_name Name or alias used to invoke the command.
     * @param options Parsed command options and arguments.
     * @param collector Optional collector used for help output.
     * @return True when help was displayed or the command completed successfully.
     */
    bool do_run(const std::string command_name, const ParsedOptions& options, const OptionsCollector* collector = nullptr) override {
        if (options.args.count("help") > 0) {
            print_help(command_name, collector);
            return true;
        }
        return run(options);
    }
};
