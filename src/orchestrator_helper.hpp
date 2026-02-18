#pragma once

#include <bits/stdc++.h>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include <arkanjo/cli/parser_options.hpp>
#include <arkanjo/orchestrator.hpp>

#include "commands/big_clone/big_clone_formater.hpp"
#include "commands/big_clone/big_clone_tailor_evaluator.hpp"
#include "commands/counter/counter_duplication_code.hpp"
#include "commands/counter/counter_duplication_code_trie.hpp"
#include "commands/explorer/similarity_explorer.hpp"
#include "commands/finder/similar_function_finder.hpp"
#include "commands/pre/preprocessor.hpp"
#include "commands/rand/random_selector.hpp"
#include "commands/help/help.hpp"

namespace OrchestratorHelper {
    const std::vector<struct option> global_long_opts = {
        {"color", no_argument, nullptr, 0},
        {"no-color", no_argument, nullptr, 0},
        {"preprocessor", no_argument, nullptr, 0},
        {"similarity", required_argument, nullptr, 's'}
    };
    const std::string global_short_opts = "s";

    inline Step setup_command_step(std::unique_ptr<ICommand>& command, Similarity_Table& table) {
        return [&table, &command](Context& ctx) {
            if (ctx.command_name == "explorer") command = std::make_unique<SimilarityExplorer>(&table);
            else if (ctx.command_name == "duplication") command = std::make_unique<CounterDuplicationCode>(&table);
            else if (ctx.command_name == "function") command = std::make_unique<SimilarFunctionFinder>(&table);
            else if (ctx.command_name == "random") command = std::make_unique<RandomSelector>(&table);
            else if (ctx.command_name == "bigclone-formater") command = std::make_unique<BigCloneFormater>(&table);
            else if (ctx.command_name == "bigclone-evaluator") command = std::make_unique<BigCloneTailorEvaluator>(&table);
            else if (ctx.command_name == "help") command = std::make_unique<Help>();
            else throw CommandNotFoundError(ctx.command_name);

            return true;
        };
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

    inline Step command_run_step(std::unique_ptr<ICommand>& command) {
        return [&command](Context& ctx) {
            return command->validate(ctx.options) && command->run(ctx.options);
        };
    }
}