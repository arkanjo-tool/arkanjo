/**
 * @file duplication_finder_tool.hpp
 * @brief Main duplication detection tool interface
 *
 * Provides the primary interface for the code duplication detection tool,
 * handling the heavy preprocessing of the input codebase to enable
 * fast query responses during operation.
 * 
 * Defines the preprocessor/setup step of the tool,
 * where we do a heavy preprocessing of the input codebase
 * to enable fast query response later.
 */

#ifndef DUPLICATION_FINDER_TOOL_HPP
#define DUPLICATION_FINDER_TOOL_HPP

#include <string>
#include <iostream>
#include "parser.hpp"
using namespace std;

/**
 * @brief Main code duplication detection tool
 *
 * Orchestrates the preprocessing pipeline that analyzes the entire codebase
 * to identify and catalog potential code duplications, creating the foundation
 * for efficient similarity queries during the tool's operation phase.
 */
class DuplicationFinderTool {
        private:
                string SAVING_MESSAGE = "Saving results...";  ///< Status message displayed when saving analysis results

                string base_path;  ///< Root directory path of the codebase to analyze
                double similarity;  ///< Similarity threshold for considering code segments duplicates (0-100 scale)

        public:
                /**
                 * @brief Constructs the duplication finder tool
                 * @param base_path_ Root path of the codebase to analyze
                 * @param similarity_ Minimum similarity threshold (0-100) to consider as duplicate
                 */
                DuplicationFinderTool(string base_path_, double similarity_);

                /**
                 * @brief Executes the full duplication analysis pipeline
                 *
                 * Performs the complete preprocessing and analysis of the codebase,
                 * including file scanning, similarity comparison, and result saving.
                 */
                void execute();
};

#endif