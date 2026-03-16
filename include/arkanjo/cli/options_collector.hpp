#pragma once

#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/orchestrator.hpp>
#include <string>
#include <vector>

class OptionsCollector {
  private:
    std::vector<struct option> merged_long_opts;

  public:
    void add_options(const option* long_opts) {
        if (long_opts != nullptr) {
            for (const option* opt = long_opts; opt->name != nullptr; ++opt) {
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
            parse_options(cmd_argc, cmd_argv, merged_long_opts.data(), ctx.options);

            return true;
        };
    }
};
