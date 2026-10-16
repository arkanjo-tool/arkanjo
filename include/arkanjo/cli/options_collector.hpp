#pragma once

#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/orchestrator.hpp>
#include <string>
#include <vector>

/**
 * @brief Collects and merges CLI options across commands and creates an option parsing step.
 */
class OptionsCollector {
  private:
    std::vector<struct CliOption> merged_long_opts;

  public:
    /**
     * @brief Adds a null-terminated array of CLI options to the merged collection.
     * @param long_opts Pointer to array of CliOption ending with OPTION_END.
     */
    void add_options(const CliOption* long_opts) {
        if (long_opts != nullptr) {
            for (const CliOption* opt = long_opts; opt->long_name != nullptr; ++opt) {
                merged_long_opts.push_back(*opt);
            }
        }
    }

    /**
     * @brief Creates an orchestration Step that parses command-line arguments into context options.
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     * @return Orchestration Step callback.
     */
    Step make_parse_step(int argc, char* argv[]) {
        return [=](Context& ctx) {
            merged_long_opts.push_back(OPTION_END);

            opterr = 0;
            char** cmd_argv = argv + 1;
            int cmd_argc = argc - 1;
            return parse_options(cmd_argc, cmd_argv, merged_long_opts, ctx.options);
        };
    }

    /**
     * @brief Returns the collection of merged CLI options.
     * @return Reference to the vector of merged CliOption.
     */
    const std::vector<CliOption>& get_options() const {
        return merged_long_opts;
    }
};
