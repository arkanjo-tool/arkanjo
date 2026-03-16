#pragma once

#include <arkanjo/commands/command.hpp>
#include <arkanjo/utils/utils.hpp>
#include <arkanjo/cli/formatter.hpp>

#define COMMAND_DESCRIPTION(str)                    \
    std::string_view description() const override { \
        static constexpr char str__[] = str;        \
        return str__;                               \
    }

template <typename, typename = std::void_t<>>
struct has_options : std::false_type {};

template <typename T>
struct has_options<T, std::void_t<decltype(T::options_)>> : std::true_type {};

template <typename Derived>
class CommandBase : public ICommand {

  public:
    virtual void print_help() const {
        std::string s(description());
        std::cout << wrapped(s, 0) << "\n";
        
        if (options()) {
            std::vector<const CliOption*> vector_arguments, vector_options;
            for (const CliOption* opt = options(); opt->long_name != nullptr; ++opt) {
                if (opt->has_arg == PositionalArgument) {
                    vector_arguments.push_back(opt);
                } else {
                    vector_options.push_back(opt);
                }
            }

            if (!vector_arguments.empty()) {
                std::cout << BOLD(UNDERLINE("Arguments:")) << "\n";

                for (const auto& item : vector_arguments) {
                    std::string opts_str = "  ";
                    opts_str += "<";
                    opts_str += to_uppercase(item->long_name);
                    opts_str += ">";
                    std::cout << std::left << std::setw(26) << opts_str;
                    if (item->description != nullptr) {
                        std::cout << wrapped(item->description, 26);
                    }
                    std::cout << "\n";
                }
            }

            if (!vector_arguments.empty() && !vector_options.empty()) std::cout << "\n";

            if (!vector_options.empty()) {
                std::cout << BOLD(UNDERLINE("Options:")) << "\n";

                for (const auto& item : vector_options) {
                    std::string opts_str = "  ";
                    if (item->short_name != 0)
                        opts_str += "-" + std::string(1, static_cast<char>(item->short_name));
                    if (!opts_str.empty() && item->long_name)
                        opts_str += ", ";
                    if (item->long_name)
                        opts_str += "--" + std::string(item->long_name);
                    opts_str = BOLD(opts_str);
                    
                    if (item->has_arg == RequiredArgument || item->has_arg == OptionalArgument) {
                        opts_str += " <" + to_uppercase(item->long_name) + "> ";
                    }
                    std::cout << std::left << std::setw(34) << opts_str;
                    if (item->description != nullptr) {
                        std::cout << wrapped(item->description, 26);
                    }
                    std::cout << "\n";
                }
            }
        }
    }

    const CliOption* options() const final {
        if constexpr (has_options<Derived>::value) {
            return Derived::options_;
        } else {
            return nullptr;
        }
    }

    bool do_run(const ParsedOptions& options) {
        if (options.args.count("help") > 0) {
            print_help();
            return true;
        }
        return run(options);
    }
};