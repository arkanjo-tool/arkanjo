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
        std::cout << description() << "\n\n";
        
        if (options()) {
            std::cout << BOLD(UNDERLINE("Options:")) << "\n";

            for (const CliOption* opt = options(); opt->long_name != nullptr; ++opt) {
                std::string opts_str = "  ";
                if (opt->short_name != 0)
                    opts_str += "-" + std::string(1, static_cast<char>(opt->short_name));
                if (!opts_str.empty() && opt->long_name)
                    opts_str += ", ";
                if (opt->long_name)
                    opts_str += "--" + std::string(opt->long_name);
                opts_str = BOLD(opts_str);
                
                if (opt->has_arg == RequiredArgument || opt->has_arg == OptionalArgument) {
                    std::string result = opt->long_name;
                    for (char &c : result) {
                        c = std::toupper((unsigned char) c);
                    }
                    opts_str += " <";
                    opts_str += result;
                    opts_str += "> ";
                }
                std::cout << std::left << std::setw(36) << opts_str;
                if (opt->description != nullptr) {
                    std::cout << wrapped(opt->description, 28);
                }
                std::cout << "\n";
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