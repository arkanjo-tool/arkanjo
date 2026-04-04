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

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/commands/command_base.hpp>

#include "similarity_explorer_entry.hpp"

/**
 * @brief Duplicate function explorer and analyzer
 *
 * Enables exploration of duplicate functions with configurable
 * filtering and sorting capabilities to identify code duplication patterns.
 */
class SimilarityExplorer : public CommandBase<SimilarityExplorer> {
  public:
    static constexpr int UNLIMITED_RESULTS = 0;      ///< Constant for unlimited results display
    static constexpr const char* EMPTY_PATTERN = ""; ///< Constant for empty search pattern

    static constexpr CliOption options_[] = {
        {"limiter", 'l', RequiredArgument, "Limits the number of results shown to the user. By default, all results are shown."},
        {"pattern", 'p', RequiredArgument, "Defines a pattern that function names must match to be included in the results. A function is considered a match if the pattern is a substring of the function's concatenated file path and name (e.g., `path/to/file.c:function_name`)."},
        {"both-match", 'b', NoArgument, "Enable both-pattern matching. By default, the pattern only needs to match one function."},
        {"sort", 's', NoArgument, "Sort results by number of duplicated lines. By default, results are sorted by the similarity metric."},
        {"cluster", 'c', NoArgument, "Print results with cluster relationships from the similarity table."},
        OPTION_END
    };
    COMMAND_DESCRIPTION(
        "Explore duplicated functions detected in the project.")

    /**
     * @brief Constructs explorer with configuration
     * @param _similarity_table Similarity data source
     */
    explicit SimilarityExplorer(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options) override;

    /**
     * @brief Handles code exploration command
     */
    bool run(const ParsedOptions& options) override;

  private:
    static constexpr const char* TEMPLATE_PROCESSED_RESULTS = 
      "Functions find: {path_a} "
      "AND {path_b}"
      ", TOTAL NUMBER LINES IN FUNCTIONS: {duplicated_lines}";
    static constexpr const char* TEMPLATE_PROCESSED_RESULTS_CLUSTERS = 
      "Function find: {path_a}"
      ", TOTAL NUMBER LINES IN FUNCTION: {duplicated_lines}";
    static constexpr const char* TEMPLATE_INITIAL_TEXT =
      "It was found a total of {found:bold} "
      "pair of duplicate functions in the codebase. Which the first "
      "{show:bold} can be found below.";
      
    int INITIAL_PROCESSED_RESULTS = 0;                                                                                    ///< Initial counter for processed results

    Similarity_Table* similarity_table;                ///< Source of similarity data
    int limit_on_results;                              ///< Maximum number of results to show
    std::string pattern_to_match;                      ///< Pattern to filter results
    bool both_path_need_to_match_pattern;              ///< Whether both paths must match pattern
    bool sorted_by_number_of_duplicated_code;          ///< Whether to sort by line count
    bool use_clusters;                                 ///< Whether clusters be printed
    int processed_results = INITIAL_PROCESSED_RESULTS; ///< Counter for processed results

    /**
     * @brief Determines number of pairs to show
     * @param number_pair_found Total pairs found
     * @return int Number to actually display
     */
    int find_number_pairs_show(int number_pair_found) const;

    /**
     * @brief Checks if paths match pattern filter
     * @param path1 First path to check
     * @param path2 Second path to check
     * @return bool True if paths pass filter
     */
    bool match_pattern(const Path& path1, const Path& path2) const;

    /**
     * @brief Gets line count for a path
     * @param path1 Path to check
     * @return int Number of lines
     */
    static int find_number_lines(const Path& path1);

    /**
     * @brief Processes a pair of similar paths
     * @param path1 First path in pair
     * @param path2 Second path in pair
     */
    SimilarityExplorerEntry process_similar_path_pair(const Path& path1, const Path& path2);

    /**
     * @brief Counts matching pairs
     * @param similar_path_pairs Pairs to check
     * @return int Number of matching pairs
     */
    int find_number_pair_found(const std::vector<std::pair<Path, Path>>& similar_path_pairs) const;

    /**
     * @brief Builds filtered path pairs
     * @return vector<pair<Path,Path>> Filtered and sorted pairs
     */
    std::vector<std::pair<Path, Path>> build_similar_path_pairs();

    /**
     * @brief Display clusters of similar functions.
     */
    void explorer_clusters();

    /**
     * @brief Main exploration driver
     */
    void explorer();
};
