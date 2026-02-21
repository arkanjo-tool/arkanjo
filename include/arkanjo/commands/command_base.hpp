#pragma once

#include <arkanjo/commands/command.hpp>

#define COMMAND_DESCRIPTION(str)                    \
    std::string_view description() const override { \
        static constexpr char str__[] = str;        \
        return str__;                               \
    }

template <typename, typename = std::void_t<>>
struct has_short_opts : std::false_type {};

template <typename T>
struct has_short_opts<T, std::void_t<decltype(T::short_opts_)>> : std::true_type {};

template <typename, typename = std::void_t<>>
struct has_options : std::false_type {};

template <typename T>
struct has_options<T, std::void_t<decltype(T::options_)>> : std::true_type {};

template <typename Derived>
class CommandBase : public ICommand {

  public:
    virtual void print_help() const {
        std::cout << "Description:\n  " << description() << "\n\n";
        if (options()) {
            std::cout << "Options:\n";

            for (const option* opt = options(); opt->name != nullptr; ++opt) {
                std::string opts;
                if (opt->val != 0)
                    opts += "-" + std::string(1, static_cast<char>(opt->val));
                if (!opts.empty() && opt->name)
                    opts += ", ";
                if (opt->name)
                    opts += "--" + std::string(opt->name);
                std::cout << "  " << opts << "\n";
            }
        }
    }

    const option* options() const final {
        if constexpr (has_options<Derived>::value) {
            return Derived::options_;
        } else {
            return nullptr;
        }
    }

    const char* short_opts() const final {
        if constexpr (has_short_opts<Derived>::value) {
            return Derived::short_opts_;
        } else {
            return "";
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