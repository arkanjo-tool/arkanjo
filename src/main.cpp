#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/cli/formatter.hpp>
#include <arkanjo/cli/options_collector.hpp>

#include <arkanjo/orchestrator.hpp>
#include "orchestrator_helper.hpp"

static constexpr const char* DEFAULT_COMMAND = "help";

int main(int argc, char* argv[]) {
    Orchestrator orchestrator;
    Context ctx;

    Similarity_Table similarity_table;

    ctx.command_name = (argc > 1) ? argv[1] : DEFAULT_COMMAND;

    std::unique_ptr<ICommand> command;
    orchestrator.add_step(OrchestratorHelper::setup_command_step(command, similarity_table));
    
    OptionsCollector collector;
    orchestrator.add_step([&command, &collector](Context& ctx) {
        if (!command) return false;

        collector.add_options(OrchestratorHelper::global_long_opts, OrchestratorHelper::global_short_opts);
        collector.add_options(command->get_options(), command->get_short_opts());
        return true;
    });

    orchestrator.add_step(collector.make_parse_step(argc, argv));

    orchestrator.add_step(OrchestratorHelper::formatter_step);

    orchestrator.add_step(OrchestratorHelper::preprocessing_step);

    orchestrator.add_step(OrchestratorHelper::similarity_step(similarity_table));

    orchestrator.add_step(OrchestratorHelper::command_run_step(command));

    try {
        orchestrator.run_pipeline(ctx);
    } catch (const CommandNotFoundError& e) {
        std::make_unique<Help>()->run(ctx.options);
        return 1;
    } catch (const CLIError& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
