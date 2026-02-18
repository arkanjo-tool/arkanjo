#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/cli/formatter.hpp>

#include "orchestrator.hpp"
#include "orchestrator_helper.hpp"

int main(int argc, char* argv[]) {
    Orchestrator orchestrator;
    Context ctx;

    Similarity_Table similarity_table;

    ctx.command_name = (argc > 1) ? argv[1] : "help";

    static struct option global_long_opts[] = {
        {"color", no_argument, nullptr, 0},
        {"no-color", no_argument, nullptr, 0},
        {"preprocessor", no_argument, nullptr, 0},
        {"similarity", required_argument, nullptr, 's'}
    };
    static std::string global_short_opts = "s";

    std::unique_ptr<ICommand> command;

    orchestrator.add_step([&](Context& c) {
        command = setup_command_step(c.command_name, similarity_table);
        return true;
    });

    orchestrator.add_step([&](Context& c) {
        return parse_options_step(argc, argv, *command, c, global_long_opts, global_short_opts);
    });

    orchestrator.add_step(formatter_step);

    orchestrator.add_step(preprocessing_step);

    orchestrator.add_step([&](Context& c) {
        return similarity_step(similarity_table, c);
    });

    orchestrator.add_step([&](Context& c) {
        return command_run_step(*command, c);
    });

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
