#pragma once

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/orchestrator.hpp>
#include <arkanjo/commands/command.hpp>
#include <arkanjo/commands/commands_registry.hpp>

namespace OrchestratorHelper {
static constexpr const char* DEFAULT_COMMAND = "help";

constexpr CliOption global_long_opts[] = {
    {"name", 'n', RequiredArgument, "Assign a name to the cache container; defaults to 'default' if not provided."},
    {"color", 0, NoArgument, "Enable colored output."},
    {"no-color", 0, NoArgument, "Disable colored output."},
    {"preprocessor", 0, NoArgument, "Forces the preprocessor to execute."},
    {"similarity", 'S', RequiredArgument, "Changes the similarity threshold to `SIMILARITY` for the current command only."},
    {"help", 'h', NoArgument, "Show this help message."},
    OPTION_END
};

inline Step setup_command_step(
    std::unique_ptr<ICommand>& command,
    const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands
) {
    return [&command, &commands](Context& ctx) mutable {
        command = CommandsRegistry::get_command(ctx.command_name, commands);
        if (command.get() != nullptr) {
            return true;
        }

        std::string external_cmd = Config::config().program_name + "-" + ctx.command_name;
        std::vector<char*> new_argv;
        new_argv.push_back(const_cast<char*>(external_cmd.c_str()));
        for (int i = 2; i < ctx.argc; ++i) {
            new_argv.push_back(ctx.argv[i]);
        }
        new_argv.push_back(nullptr);
        UtilsOSDependable::run_process(external_cmd.c_str(), new_argv.data());

        throw CommandNotFoundError(ctx.command_name);

        return true;
    };
}

inline bool formatter_step(Context& ctx) {
    bool color = ctx.options.args.count("color") > 0;
    if (color)
        FormatterManager::set_formatter(std::make_shared<ConsoleFormatter>(true));
    bool no_color = ctx.options.args.count("no-color") > 0;
    if (no_color)
        FormatterManager::set_formatter(std::make_shared<ConsoleFormatter>(false));
    return true;
}

inline Step similarity_step(Similarity_Table& table) {
    return [&table](Context& ctx) {
        table.load();
        auto it = ctx.options.args.find("similarity");
        if (it != ctx.options.args.end()) {
            double sim = std::stod(ctx.options.args["similarity"]);
            table.update_similarity(sim);
        }
        return true;
    };
}

inline Step command_run_step(std::shared_ptr<ICommand> command, const OptionsCollector& collector) {
    return [cmd = std::move(command), &collector](Context& ctx) mutable {
        return cmd->validate(ctx.options) && cmd->do_run(ctx.command_name, ctx.options, &collector);
    };
}
} // namespace OrchestratorHelper