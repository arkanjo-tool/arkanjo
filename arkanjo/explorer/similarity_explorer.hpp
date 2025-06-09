/**
 * @file similarity_explorer.hpp
 * @brief Duplicate function exploration interface
 *
 * Provides functionality to explore and analyze duplicate functions
 * with various filtering and sorting options.
 *
 * The Similarity Explorer implements the functionality to 
 * implement to find duplicated functions enabling some
 * filters and sortings options
 */

#ifndef SIMILARITY_EXPLORER_HPP
#define SIMILARITY_EXPLORER_HPP

#include <string>
#include <vector>
#include <utility>

#include "../base/path.hpp"
#include "../base/function.hpp"
#include "../base/utils.hpp"
#include "../base/similarity_table.hpp"
using namespace std;

/**
 * @brief Duplicate function explorer and analyzer
 *
 * Enables exploration of duplicate functions with configurable
 * filtering and sorting capabilities to identify code duplication patterns.
 */
class Similarity_Explorer {
        public:
                int UNLIMITED_RESULTS = 0;       ///< Constant for unlimited results display
                string EMPTY_PATTERN = "";       ///< Constant for empty search pattern

        private:
                string BETWEEN_RELATIVE_AND_FUNCTION_NAME = "::";  ///< Separator for path formatting
                string START_LINE_COMPARATION_PRINT = "Functions find: ";  ///< Comparison header
                string BETWEEN_TWO_FUNCTION = " AND ";             ///< Separator between functions
                string NUMBER_LINES_MESSAGE = " , TOTAL NUMBER LINES IN FUNCTIONS: ";  ///< Line count label
                string INITIAL_TEXT_PRINT_1 = "It was found a total of ";  ///< Results message part 1
                string INITIAL_TEXT_PRINT_2 = " pair of duplicate functions in the codebase. Which the first ";  ///< Results message part 2
                string INITIAL_TEXT_PRINT_3 = " can be found below.";  ///< Results message part 3
                int INITIAL_PROCESSED_RESULTS = 0;  ///< Initial counter for processed results

                Similarity_Table *similarity_table;  ///< Source of similarity data
                int limit_on_results;               ///< Maximum number of results to show
                string pattern_to_match_path;       ///< Pattern to filter path names
                string pattern_to_match_function;        ///< Pattern to filter function names
                bool both_path_need_to_match_pattern;  ///< Whether both paths must match pattern
                int processed_results = INITIAL_PROCESSED_RESULTS;  ///< Counter for processed results

                /**
                 * @brief Chooses text color for output
                 * @return Utils::COLOR Selected color
                 */
                Utils::COLOR choose_text_color();

                /**
                 * @brief Determines number of pairs to show
                 * @param number_pair_found Total pairs found
                 * @return int Number to actually display
                 */
                int find_number_pairs_show(int number_pair_found);

                /**
                 * @brief Formats initial results message
                 * @param number_pair_found Total pairs found
                 * @return string Formatted message
                 */
                string format_initial_message(int number_pair_found);

                /**
                 * @brief Checks if paths match pattern filter
                 * @param path1 First path to check
                 * @param path2 Second path to check
                 * @return bool True if paths pass filter
                 */
                bool match_pattern(Path path1, Path path2);

                /**
                 * @brief Formats path for display
                 * @param path Path to format
                 * @return string Formatted path string
                 */
                string format_path_message_in_pair(Path path);

                /**
                 * @brief Gets line count for a path
                 * @param path1 Path to check
                 * @return int Number of lines
                 */
                int find_number_lines(Path path1);

                /**
                 * @brief Prints a pair of similar paths
                 * @param path1 First path in pair
                 * @param path2 Second path in pair
                 */
                void print_similar_path_pair(Path path1, Path path2);

                /**
                 * @brief Processes a pair of similar paths
                 * @param path1 First path in pair
                 * @param path2 Second path in pair
                 */
                void process_similar_path_pair(Path path1, Path path2);

                /**
                 * @brief Counts matching pairs
                 * @param similar_path_pairs Pairs to check
                 * @return int Number of matching pairs
                 */
                int find_number_pair_found(vector<pair<Path,Path>> similar_path_pairs);

                /**
                 * @brief Builds filtered path pairs
                 * @param sorted_by_number_of_duplicated_code Whether to sort by line count
                 * @return vector<pair<Path,Path>> Filtered and sorted pairs
                 */
                vector<pair<Path,Path>> build_similar_path_pairs(bool sorted_by_number_of_duplicated_code);

                /**
                 * @brief Main exploration driver
                 * @param sorted_by_number_of_duplicated_code Whether to sort by line count
                 */
                void explorer(bool sorted_by_number_of_duplicated_code);

        public:
                /**
                 * @brief Constructs explorer with configuration
                 * @param _similarity_table Similarity data source
                 * @param _limit_on_results Maximum results to show
                 * @param _pattern_to_match_path Filter pattern
                 * @param _pattern_to_match_function Function name filter
                 * @param _both_path_need_to_match Whether both paths must match pattern
                 * @param sorted_by_number_of_duplicated_code Whether to sort by line count
                 */
                Similarity_Explorer(Similarity_Table *_similarity_table,
                        int _limit_on_results,
                        string _pattern_to_match_path, 
                        string _pattern_to_match_function, 
                        bool _both_path_need_to_match,
                        bool sorted_by_number_of_duplicated_code=false);
};

#endif