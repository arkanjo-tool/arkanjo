#pragma once

#include <arkanjo/orchestrator.hpp>
#include <arkanjo/cli/parser_options.hpp>
#include <vector>
#include <string>

class OptionsCollector {
private:
    std::vector<struct option> merged_long_opts;
    std::string merged_short_opts;

public:
    void add_options(const std::vector<option>& long_opts, const std::string& short_opts) {
        if (long_opts.size() > 0) {
            for (int i = 0; long_opts[i].name != nullptr; i++) {
                merged_long_opts.push_back(long_opts[i]);
            }
        }
        merged_short_opts += short_opts;
    }

    Step make_parse_step(int argc, char* argv[]) {
        return [=](Context& ctx) {
            merged_long_opts.push_back(OPTION_END);

            opterr = 0;
            char** cmd_argv = argv + 1;
            int cmd_argc = argc - 1;
            parse_options(cmd_argc, cmd_argv, merged_short_opts, merged_long_opts.data(), ctx.options);

            return true;
        };
    }
};
