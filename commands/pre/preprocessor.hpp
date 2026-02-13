/**
 * @file preprocessor.hpp
 * @brief Codebase preprocessing interface
 *
 * Defines the preprocessing stage that prepares the codebase for
 * efficient duplication detection by parsing, analyzing, and organizing
 * source code into a queryable format.
 * 
 * Defines the preprocessor/setup step of the tool, where we do a heavy
 * preprocessing of the input codebase to enable fast query response later.
 */

#pragma once

#include <string>
#include <tuple>
#include "parser.hpp"
#include "function_breaker.hpp"
#include <arkanjo/base/config.hpp> 
#include "duplication_finder_tool.hpp"
#include "duplication_finder_diff.hpp"
using namespace std;

/**
 * @brief Codebase preprocessing orchestrator
 *
 * Manages the complete preprocessing pipeline including:
 * - Code structure analysis
 * - Function extraction
 * - Duplication detection
 * - Results organization
 *
 * Creates the foundation for fast query responses during the main operation phase.
 */
class Preprocessor {
        private:
                // User interaction messages
                string PROJECT_PATH_MESSAGE = "Enter your project path:";  ///< Project path prompt
                string MINIMUM_SIMILARITY_MESSAGE = "Enter minimum similarity desired on using the tool:";  ///< Similarity threshold prompt
                string CONFIG_PATH = "config.txt";                        ///< Configuration file path
                string PATH_MESSAGE = "path of the current preprocess: "; ///< Current processing path display
                string TIME_MESSAGE = "Finished time: ";                  ///< Timing information prefix

                // Processing stage messages
                string INITIAL_MESSAGE = "Initiating Preprocessing";      ///< Initialization message
                string BREAKER_MESSAGE = "Reading codebase... (this may take a while)";  ///< Code analysis message
                string DUPLICATION_MESSAGE = "Finding duplication in the codebase... (this may take a while)";  ///< Duplication detection message
                string SAVING_MESSAGE = "Saving results...";              ///< Results saving message
                string END_MESSAGE = "Finished preprocessing";            ///< Completion message

                // Duplication finder selection messages
                string MESSAGE_DUPLICATION_FINDER_TYPE_1 = "Enter the number of the duplication finder technique you want to use:";
                string MESSAGE_DUPLICATION_FINDER_TYPE_2 = "1) NLP text similarity using gensim";
                string MESSAGE_DUPLICATION_FINDER_TYPE_3 = "2) Count proportion of equal lines using diff command";
                string INVALID_CODE_DUPLICATION_FINDER = "Valid options are '1' or '2' only. Stopping Program...";

                /**
                 * @brief Reads preprocessing parameters from user/config
                 * @return tuple<string,double,bool> 
                 *         - Project path
                 *         - Similarity threshold
                 *         - Duplication finder selection flag
                 */
                tuple<string,double,bool> read_parameters();

                /**
                 * @brief Saves preprocessing parameters for future runs
                 * @param path Project path to save
                 */
                void save_current_run_params(string path);

                /**
                 * @brief Executes full preprocessing pipeline
                 * @param path Project path to process
                 * @param similarity Similarity threshold
                 * @param use_duplication_finder_by_tool Flag to select duplication detection method
                 */
                void preprocess(string path, double similarity, bool use_duplication_finder_by_tool);

        public:
                /**
                 * @brief Constructs preprocessor with optional forcing
                 * @param force_preprocess Whether to force preprocessing even if cached results exist
                 */
                Preprocessor(bool force_preprocess);
                
                /**
                 * @brief Testing constructor with direct parameter specification
                 * @param force_preprocess Whether to force preprocessing
                 * @param path Direct project path specification
                 * @param similarity Direct similarity threshold specification
                 * @note Exposed for testing purposes only
                 */
                Preprocessor(bool force_preprocess, string path, double similarity);
};
