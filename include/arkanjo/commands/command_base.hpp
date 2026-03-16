#pragma once

#include <arkanjo/commands/command.hpp>
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
        std::string out;

        out += description();
        out += "\n\n";
        if (options()) {
            out += BOLD(UNDERLINE("Options:"));
            out += "\n";

            for (const option* opt = options(); opt->name != nullptr; ++opt) {
                std::string opts_str;
                if (opt->val != 0)
                    opts_str += "-" + std::string(1, static_cast<char>(opt->val));
                if (!opts_str.empty() && opt->name)
                    opts_str += ", ";
                if (opt->name)
                    opts_str += "--" + std::string(opt->name);
                out += "  ";
                out += BOLD(opts_str);
                
                if (opt->has_arg == required_argument || opt->has_arg == optional_argument) {
                    std::string result = opt->name;
                    for (char &c : result) {
                        c = std::toupper((unsigned char) c);
                    }
                    out += " <";
                    out += result;
                    out += ">";
                }
                out += "\n";
            }
        }
        std::cout << out;
    }

    const option* options() const final {
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