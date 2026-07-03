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

#include <arkanjo/base/function/function.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/base/similarity_table.hpp>

#include <arkanjo/cli/parser_options.hpp>

/**
 * @brief Mutable state shared by orchestrator pipeline steps.
 */
struct Context {
    std::string command_name;        ///< Command name selected by the user.
    ParsedOptions options;           ///< Parsed options for the current command.
    std::vector<std::string> extra_args; ///< Arguments forwarded to external commands.

    int argc;   ///< Original process argument count.
    char** argv; ///< Original process argument vector.
};

/**
 * @brief Pipeline step that can inspect or update the command context.
 *
 * Steps return true to continue the pipeline and false to stop it.
 */
using Step = std::function<bool(Context&)>;
/**
 * @brief Main command orchestrator
 *
 * It creates and adds steps to the pipeline.
 */
class Orchestrator {
  private:
    std::vector<Step> steps;
    size_t current_step = 0;

  public:
    /**
     * @brief Adds a step to the end of the pipeline.
     * @param step Step callback to execute.
     */
    void add_step(Step step) { steps.push_back(step); }

    /**
     * @brief Skips all remaining pipeline steps.
     */
    void skip() {
        current_step = steps.size();
    }

    /**
     * @brief Runs each pipeline step until completion or failure.
     * @param ctx Context shared by all pipeline steps.
     */
    void run_pipeline(Context& ctx) {
        for (current_step = 0; current_step < steps.size(); ++current_step) {
            if (!steps[current_step] || !steps[current_step](ctx)) {
                std::cerr << "Pipeline aborted (" << current_step << ")\n";
                break;
            }
        }
    }
};
