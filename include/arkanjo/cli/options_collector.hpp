#pragma once

#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/orchestrator.hpp>
#include <string>
#include <vector>

class OptionsCollector {
  private:
    std::vector<struct CliOption> merged_long_opts;

  public:
    void add_options(const CliOption* long_opts) {
        if (long_opts != nullptr) {
            for (const CliOption* opt = long_opts; opt->long_name != nullptr; ++opt) {
                merged_long_opts.push_back(*opt);
            }
        }
    }

    Step make_parse_step(int argc, char* argv[]) {
        return [=](Context& ctx) {
            merged_long_opts.push_back(OPTION_END);

            opterr = 0;
            char** cmd_argv = argv + 1;
            int cmd_argc = argc - 1;
            return parse_options(cmd_argc, cmd_argv, merged_long_opts, ctx.options);
        };
    }

    const std::vector<CliOption>& get_options() const {
        return merged_long_opts;
    }
};
