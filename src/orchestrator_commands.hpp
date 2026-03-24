#pragma once

#include <arkanjo/orchestrator.hpp>

#include "commands/big_clone/big_clone_formater.hpp"
#include "commands/big_clone/big_clone_tailor_evaluator.hpp"
#include "commands/counter/counter_duplication_code.hpp"
#include "commands/counter/counter_duplication_code_trie.hpp"
#include "commands/explorer/similarity_explorer.hpp"
#include "commands/finder/similar_function_finder.hpp"
#include "commands/help/help.hpp"
#include "commands/pre/preprocessor.hpp"
#include "commands/rand/random_selector.hpp"

namespace OrchestratorCommands {
    static constexpr const char* DEFAULT_COMMAND = "help";

    template<typename Table>
    using CommandMap = std::unordered_map<std::string, std::function<std::unique_ptr<ICommand>()>>;

    template<typename Table>
    inline CommandMap<Table> create_internal_commands(Table& table) {
        CommandMap<Table> commands = {
            {"explorer", [&]() { return std::make_unique<SimilarityExplorer>(&table); }},
            {"duplication", [&]() { return std::make_unique<CounterDuplicationCode>(&table); }},
            {"function", [&]() { return std::make_unique<SimilarFunctionFinder>(&table); }},
            {"random", [&]() { return std::make_unique<RandomSelector>(&table); }},
            {"bigclone-formater", [&]() { return std::make_unique<BigCloneFormater>(&table); }},
            {"bigclone-evaluator", [&]() { return std::make_unique<BigCloneTailorEvaluator>(&table); }}
        };

        // Adiciona Help agora que o map existe
        commands["help"] = [&]() { return std::make_unique<Help>(commands); };

        return commands;
    }
} // namespace OrchestratorCommands