/**
 * @file orchestrator.hpp
 * @brief Main command orchestration interface
 *
 * Provides the central command dispatch system that coordinates all
 * tool functionality through various subcomponents.
 *
 * The orchestrator starts the flow of the user iteraction
 * read the user cli command, parser it and call the correct
 * functionality related to the user query.
 */

#include <bits/stdc++.h>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include "commands/big_clone/big_clone_formater.hpp"
#include "commands/big_clone/big_clone_tailor_evaluator.hpp"
#include "commands/counter/counter_duplication_code.hpp"
#include "commands/counter/counter_duplication_code_trie.hpp"
#include "commands/explorer/similarity_explorer.hpp"
#include "commands/finder/similar_function_finder.hpp"
#include "commands/pre/preprocessor.hpp"
#include "commands/rand/random_selector.hpp"
using namespace std;

/**
 * @brief Main command orchestrator
 *
 * Routes and executes tool commands by coordinating between various
 * subsystems including preprocessing, exploration, duplication counting,
 * and evaluation components.
 */
class Orchestrator {
  private:
    /**
     * @brief Displays help information about available commands
     */
    void help_command();

    /**
     * @brief Updates similarity threshold if specified in parameters
     * @param parameters Command line parameters
     * @param similarity_table Similarity table to modify
     */
    void check_update_similarity(vector<string> parameters, Similarity_Table* similarity_table);

    /**
     * @brief Checks if force preprocessing was requested
     * @param parameters Command line parameters
     * @return bool True if force preprocessing was requested
     */
    bool check_force_preprocess(vector<string> parameters);

    /**
     * @brief Executes preprocessing pipeline
     * @param parameters Command line parameters
     */
    void call_preprocess(vector<string> parameters);

    /**
     * @brief Handles code exploration command
     * @param parameters Command line parameters
     * @param similarity_table Similarity data to explore
     */
    void exploration_command(vector<string> parameters, Similarity_Table* similarity_table);

    /**
     * @brief Handles random selection command
     * @param parameters Command line parameters
     * @param similarity_table Similarity data to sample from
     */
    void random_command(vector<string> parameters, Similarity_Table* similarity_table);

    /**
     * @brief Handles duplication analysis command
     * @param parameters Command line parameters
     * @param similarity_table Similarity data to analyze
     */
    void duplication_command(vector<string> parameters, Similarity_Table* similarity_table);

    /**
     * @brief Handles BigCloneEval formatting command
     * @param parameters Command line parameters
     * @param similarity_table Similarity data to format
     */
    void big_clone_formater_command(vector<string> parameters, Similarity_Table* similarity_table);

    /**
     * @brief Handles BigCloneBench evaluation command
     * @param parameters Command line parameters
     * @param similarity_table Similarity data to evaluate
     */
    void big_clone_tailor_evaluator_command(vector<string> parameters, Similarity_Table* similarity_table);

    /**
     * @brief Handles similar function finding command
     * @param parameters Command line parameters
     * @param similarity_table Similarity data to search
     */
    void similar_function_finder_command(vector<string> parameters, Similarity_Table* similarity_table);

  public:
    /**
     * @brief Constructs orchestrator and executes requested command
     * @param command The main command to execute
     * @param parameters Additional command parameters
     */
    Orchestrator(string command, vector<string> parameters);
};