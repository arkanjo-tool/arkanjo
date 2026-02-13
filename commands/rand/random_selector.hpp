/**
 * @file random_selector.hpp
 * @brief Random selection of similar code pairs
 *
 * Provides functionality to randomly select and display pairs of similar
 * code segments within specified similarity ranges.
 * 
 * The Random Selector random selects a set of duplicated functions pairs
 * enables to set the interval of acceptable similarity probability and 
 * the number of functions to be selected.
 */

#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <random>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/utils.hpp>
#include "../base/similarity_table.hpp"
using namespace std;

/**
 * @brief Random pair selector for similar code segments
 *
 * Selects and displays random pairs of similar functions within
 * configurable similarity thresholds, with formatted output and
 * color-coded display options.
 */
class Random_Selector {
        string START_LINE_COMPARATION_PRINT = "Functions: ";          ///< Prefix for pair display
        string BETWEEN_TWO_FUNCTION = " AND ";                       ///< Separator between functions
        string BETWEEN_RELATIVE_AND_FUNCTION_NAME = "::";            ///< Path/name separator
        string SIMILARITY_MESSAGE = ". Similarity: ";                ///< Similarity score prefix
        
        Similarity_Table *similarity_table;                          ///< Source of similarity data
        double minimum_similarity;                                   ///< Minimum similarity threshold
        double maximum_similarity;                                   ///< Maximum similarity threshold
        double maximum_quantity;                                     ///< Maximum number of pairs to show
        
        const int seed = 123456789;                                  ///< Fixed random seed for reproducibility
        mt19937 rng = mt19937(seed);                                 ///< Mersenne Twister random generator
        
        int processed_results = 0;                                   ///< Counter for processed pairs

        /**
         * @brief Selects text color based on similarity score
         * @return Utils::COLOR Selected color for display
         */
        Utils::COLOR choose_text_color();

        /**
         * @brief Formats path for display
         * @param path Path to format
         * @return string Formatted path string
         */
        string format_path_message_in_pair(Path path);

        /**
         * @brief Checks if pair falls within configured thresholds
         * @param path_pair Tuple of similarity and paths
         * @return bool True if pair meets criteria
         */
        bool is_valid_pair(tuple<double,Path,Path> path_pair);

        /**
         * @brief Gets all pairs within similarity thresholds
         * @return vector<tuple<double,Path,Path>> Filtered pairs
         */
        vector<tuple<double,Path,Path>> get_similarity_pairs_filtered();

        /**
         * @brief Performs random selection from pairs
         * @param path_pairs Full set of candidate pairs
         * @return vector<tuple<double,Path,Path>> Randomly selected subset
         */
        vector<tuple<double,Path,Path>> make_random_selection(vector<tuple<double,Path,Path>> path_pairs);

        /**
         * @brief Prints a single path pair with formatting
         * @param path_pair Pair to display
         */
        void print_path_pair(tuple<double,Path,Path> path_pair);

        /**
         * @brief Prints all selected path pairs
         * @param path_pairs Pairs to display
         */
        void print_path_pairs(vector<tuple<double,Path,Path>> path_pairs);

        public:
                /**
                 * @brief Constructs selector with configuration
                 * @param _similarity_table Source of similarity data
                 * @param _minimum_similarity Minimum similarity threshold (0-100)
                 * @param _maximum_similarity Maximum similarity threshold (0-100)
                 * @param _maximum_quantity Maximum number of pairs to select
                 */
                Random_Selector(
                        Similarity_Table *_similarity_table,
                        double _minimum_similarity,
                        double _maximum_similarity,
                        double _maximum_quantity
                );
};
