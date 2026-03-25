#pragma once

#include <arkanjo/orchestrator.hpp>

#include "commands/big_clone/big_clone_formater.hpp"
#include "commands/big_clone/big_clone_tailor_evaluator.hpp"
#include "commands/counter/counter_duplication_code.hpp"
#include "commands/counter/counter_duplication_code_trie.hpp"
#include "commands/explorer/similarity_explorer.hpp"
#include "commands/finder/similar_function_finder.hpp"
#include "commands/help/help.hpp"
#include "commands/rand/random_selector.hpp"

namespace OrchestratorCommands {
    static constexpr const char* DEFAULT_COMMAND = "help";

    template<typename Table>
    using CommandMap = const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>;

    template<typename Table>
    inline CommandMap<Table> create_internal_commands(Table& table) {
        CommandMap<Table> commands = {
            {{"explorer", "ex"}, [&]() { return std::make_unique<SimilarityExplorer>(&table); }},
            {{"duplication", "du"}, [&]() { return std::make_unique<CounterDuplicationCode>(&table); }},
            {{"function", "fu"}, [&]() { return std::make_unique<SimilarFunctionFinder>(&table); }},
            {{"random"}, [&]() { return std::make_unique<RandomSelector>(&table); }},
            {{"bigclone-formater"}, [&]() { return std::make_unique<BigCloneFormater>(&table); }},
            {{"bigclone-evaluator"}, [&]() { return std::make_unique<BigCloneTailorEvaluator>(&table); }},
            {{"help"}, [&]() { return std::make_unique<Help>(commands); }},
            {{"preprocessor"}, [&]() { return nullptr; }}
        };

        return commands;
    }
} // namespace OrchestratorCommands