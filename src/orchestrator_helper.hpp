#pragma once

#include <bits/stdc++.h>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include <arkanjo/cli/parser_options.hpp>
#include "orchestrator.hpp"

#include "commands/big_clone/big_clone_formater.hpp"
#include "commands/big_clone/big_clone_tailor_evaluator.hpp"
#include "commands/counter/counter_duplication_code.hpp"
#include "commands/counter/counter_duplication_code_trie.hpp"
#include "commands/explorer/similarity_explorer.hpp"
#include "commands/finder/similar_function_finder.hpp"
#include "commands/pre/preprocessor.hpp"
#include "commands/rand/random_selector.hpp"
#include "commands/help/help.hpp"

std::unique_ptr<ICommand> setup_command_step(const std::string& cmd_name, Similarity_Table& table) {
    if (cmd_name == "explorer") return std::make_unique<SimilarityExplorer>(&table);
    if (cmd_name == "duplication") return std::make_unique<CounterDuplicationCode>(&table);
    if (cmd_name == "function") return std::make_unique<SimilarFunctionFinder>(&table);
    if (cmd_name == "random") return std::make_unique<RandomSelector>(&table);
    if (cmd_name == "bigclone-formater") return std::make_unique<BigCloneFormater>(&table);
    if (cmd_name == "bigclone-evaluator") return std::make_unique<BigCloneTailorEvaluator>(&table);
    if (cmd_name == "help") return std::make_unique<Help>();
    
    throw CommandNotFoundError(cmd_name);
}

bool parse_options_step(int argc, char* argv[], ICommand& command, Context& ctx, const struct option global_long_opts[], const std::string& global_short_opts) 
{
    std::vector<struct option> merged_opts;
    std::string merged_short_opts;

    for (int i = 0; global_long_opts[i].name != nullptr; i++) {
        merged_opts.push_back(global_long_opts[i]);
    }
    merged_short_opts += global_short_opts;
    
    if (command.get_options().size() > 0) {
        for (int i = 0; command.get_options()[i].name != nullptr; i++) {
            merged_opts.push_back(command.get_options()[i]);
        }
    }
    merged_short_opts += command.get_short_opts();

    merged_opts.push_back(OPTION_END);

    opterr = 0;
    char** cmd_argv = argv + 1;
    int cmd_argc = argc - 1;
    parse_options(cmd_argc, cmd_argv, merged_short_opts, merged_opts.data(), ctx.options);

    return true;
}

bool formatter_step(Context& ctx) {
    bool color = ctx.options.args.count("color") > 0;
    if (color) FormatterManager::set_formatter(std::make_shared<ConsoleFormatter>(true));
    bool no_color = ctx.options.args.count("no-color") > 0;
    if (no_color) FormatterManager::set_formatter(std::make_shared<ConsoleFormatter>(false));
    return true;
}

bool preprocessing_step(Context& ctx) {
    bool force_pre = ctx.options.args.count("preprocessor") > 0;
    Preprocessor pre(force_pre);
    return true;
}

bool similarity_step(Similarity_Table& table, Context& ctx) {
    table.load();
    auto it = ctx.options.args.find("similarity");
    if (it != ctx.options.args.end()) {
        double sim = std::stod(ctx.options.args["similarity"]);
        table.update_similarity(sim);
    }
    return true;
}

bool command_run_step(ICommand& command, Context& ctx) {
    return command.validate(ctx.options) && command.run(ctx.options);
}