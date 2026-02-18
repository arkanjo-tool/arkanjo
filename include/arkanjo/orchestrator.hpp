/**
 * @file orchestrator.hpp
 * @brief Main command orchestration interface
 *
 * Provides the central command dispatch system that coordinates all
 * tool functionality through various subcomponents.
 *
 * The orchestrator starts the flow of the user iteraction
 * read the user cli command, parser it and call the correct
 * functionality related to the user query.
 */

#pragma once

#include <bits/stdc++.h>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include <arkanjo/cli/parser_options.hpp>

struct Context {
    std::string command_name;
    ParsedOptions options;
    std::vector<std::string> extra_args;
};

using Step = std::function<bool(Context&)>;
/**
 * @brief Main command orchestrator
 *
 * It creates and adds steps to the pipeline.
 */
class Orchestrator {
  private:
    std::vector<Step> steps;

  public:
    void add_step(Step step) { steps.push_back(step); };

    void run_pipeline(Context& ctx) {
        for (size_t i = 0; i < steps.size(); ++i) {
            if (!steps[i](ctx)) {
                std::cerr << "Pipeline aborted\n";
                break;
            }
        }
    };
};