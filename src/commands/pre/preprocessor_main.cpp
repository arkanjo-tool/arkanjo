// As preprocessor is, in the moment, separated from orchestrator
// we define a main function to use it.

#include "../help/help.hpp"
#include "build/preprocessor_build.hpp"
#include "clean/preprocessor_clean.hpp"
#include "list/preprocessor_list.hpp"
#include "path/preprocessor_path.hpp"
#include <cassert>
#include <filesystem>
#include <string>

#include "cli/formatter/format_manager.hpp"
#include "commands/command.hpp"
#include "orchestrator.hpp"
#include "orchestrator_helper.hpp"

int main(int argc, char* argv[]) {
    auto& cfg = Config::config();
    try {
        cfg.setDefaultConfig();
        cfg.program_name = "arkanjo-preprocessor";
    } catch (const std::exception& e) {
        FormatterManager::error(e.what());
        return 1;
    }

    Orchestrator orchestrator;
    Context ctx;

    ctx.command_name = (argc > 1) ? argv[1] : OrchestratorHelper::kDefaultCommand;

    ctx.argc = argc;
    ctx.argv = argv;

    static const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>> internal_commands = {
        {{"build"}, [&]() { return std::make_unique<PreprocessorBuild>(); }},
        {{"list", "ls"}, [&]() { return std::make_unique<PreprocessorList>(); }},
        {{"clean"}, [&]() { return std::make_unique<PreprocessorClean>(); }},
        {{"path"}, [&]() { return std::make_unique<PreprocessorPath>(); }},
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

    orchestrator.add_step([&orchestrator, &command, &collector](Context& _ctx) {
        if (auto name_value = _ctx.options.get("name"))
            Config::config().name_container = *name_value;

        orchestrator.add_step(OrchestratorHelper::command_run_step(std::move(command), collector));

        return true;
    });

    try {
        orchestrator.run_pipeline(ctx);
    } catch (const CommandNotFoundError&) {
        if (ctx.command_name != "help" && !ctx.options.has("help"))
            FormatterManager::warn(ctx.command_name + " is not a " + Config::config().program_name + " command.");
        std::make_unique<Help>(internal_commands)->do_run(ctx.command_name, ctx.options);
        return 1;
    } catch (const CLIError& e) {
        FormatterManager::error(e.what());
        return 1;
    } catch (const std::exception& e) {
        FormatterManager::error(e.what(), "Unexpected error: ");
        return 1;
    }
    return 0;
}
