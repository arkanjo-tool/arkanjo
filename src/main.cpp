#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/cli/formatter.hpp>
#include <arkanjo/cli/options_collector.hpp>
#include <arkanjo/cli/parser_options.hpp>

#include <arkanjo/orchestrator_helper.hpp>
#include <arkanjo/orchestrator.hpp>
#include "orchestrator_commands.hpp"
#include "commands/pre/build/preprocessor.hpp"

int main(int argc, char* argv[]) {
    auto& cfg = Config::config();
    cfg.setDefaultConfig();

    Orchestrator orchestrator;
    Context ctx;

    Similarity_Table similarity_table;

    ctx.command_name = (argc > 1) ? argv[1] : OrchestratorHelper::DEFAULT_COMMAND;

    ctx.argc = argc;
    ctx.argv = argv;

    std::unique_ptr<ICommand> command;
    auto internal_commands = OrchestratorCommands::create_internal_commands(similarity_table);
    orchestrator.add_step(OrchestratorHelper::setup_command_step(command, internal_commands));

    OptionsCollector collector;
    orchestrator.add_step([&command, &collector](Context&) {
        if (!command)
            return false;

        collector.add_options(OrchestratorHelper::global_long_opts);
        collector.add_options(command->options());

        return true;
    });

    orchestrator.add_step(collector.make_parse_step(argc, argv));

    orchestrator.add_step(OrchestratorHelper::formatter_step);

    orchestrator.add_step([&similarity_table, &orchestrator, &command, &collector](Context& ctx) {
        if (ctx.command_name != "help" && ctx.options.args.count("help") == 0) {
            orchestrator.add_step([](Context& ctx) {
                bool force_pre = ctx.options.args.count("preprocessor") > 0;
                Preprocessor pre(force_pre);
                return true;
            });
            orchestrator.add_step(OrchestratorHelper::similarity_step(similarity_table));
        }

        orchestrator.add_step(OrchestratorHelper::command_run_step(std::move(command), collector));

        return true;
    });

    try {
        orchestrator.run_pipeline(ctx);
    } catch (const CommandNotFoundError& e) {
        if (ctx.command_name != "help" && ctx.options.args.count("help") == 0)
            std::cerr << "Warning: '" << ctx.command_name << "' is not a " << Config::config().program_name << " command.\n";
        std::make_unique<Help>(internal_commands)->do_run(ctx.command_name, ctx.options);
        return 1;
    } catch (const CLIError& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
