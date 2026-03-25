// As preprocessor is, in the moment, separated from orchestrator
// we define a main function to use it.

#include "build/preprocessor.hpp"
#include "../help/help.hpp"
#include <cassert>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>

#include <arkanjo/orchestrator_helper.hpp>
#include <arkanjo/orchestrator.hpp>
#include <arkanjo/commands/command.hpp>
using namespace std;

int main(int argc, char* argv[]) {
    auto& cfg = Config::config();
    cfg.setDefaultConfig();
    cfg.program_name = "arkanjo-preprocessor";

    Orchestrator orchestrator;
    Context ctx;

    ctx.command_name = (argc > 1) ? argv[1] : OrchestratorHelper::DEFAULT_COMMAND;
    
    ctx.argc = argc;
    ctx.argv = argv;

    static const std::unordered_map<std::string,
        std::function<std::unique_ptr<ICommand>()>> internal_commands = {
        {"build", [&]() { return std::make_unique<Preprocessor>(); }}
    };

    std::unique_ptr<ICommand> command;
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

    orchestrator.add_step([&orchestrator, &command, &collector](Context&) {
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
