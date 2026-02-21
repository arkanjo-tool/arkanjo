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
#include <arkanjo/cli/formatter.hpp>
#include <arkanjo/commands/command_base.hpp>

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

    static constexpr option options_[] = {
        {"limiter", required_argument, nullptr, 'l'},
        {"pattern", required_argument, nullptr, 'p'},
        {"both-match", no_argument, nullptr, 'b'},
        {"sort", no_argument, nullptr, 'c'},
        OPTION_END};
    static constexpr const char* short_opts_ = "l:p:bc";
    COMMAND_DESCRIPTION(
        "Explore duplicated functions detected in the project. "
        "Allows limiting the number of results, filtering by substring "
        "match against the full function identifier (path:name), "
        "requiring both functions to match the pattern, "
        "and sorting results by similarity or duplicated line count.")

    /**
     * @brief Constructs explorer with configuration
     * @param _similarity_table Similarity data source
     */
    explicit SimilarityExplorer(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options);

    /**
     * @brief Handles code exploration command
     */
    bool run(const ParsedOptions& options) override;

  private:
    static constexpr const char* START_LINE_COMPARATION_PRINT = "Functions find: ";                                       ///< Comparison header
    static constexpr const char* BETWEEN_TWO_FUNCTION = " AND ";                                                          ///< Separator between functions
    static constexpr const char* NUMBER_LINES_MESSAGE = " , TOTAL NUMBER LINES IN FUNCTIONS: ";                           ///< Line count label
    static constexpr const char* INITIAL_TEXT_PRINT_1 = "It was found a total of ";                                       ///< Results message part 1
    static constexpr const char* INITIAL_TEXT_PRINT_2 = " pair of duplicate functions in the codebase. Which the first "; ///< Results message part 2
    static constexpr const char* INITIAL_TEXT_PRINT_3 = " can be found below.";                                           ///< Results message part 3
    int INITIAL_PROCESSED_RESULTS = 0;                                                                                    ///< Initial counter for processed results

    Similarity_Table* similarity_table;                ///< Source of similarity data
    int limit_on_results;                              ///< Maximum number of results to show
    std::string pattern_to_match;                      ///< Pattern to filter results
    bool both_path_need_to_match_pattern;              ///< Whether both paths must match pattern
    bool sorted_by_number_of_duplicated_code;          ///< Whether to sort by line count
    int processed_results = INITIAL_PROCESSED_RESULTS; ///< Counter for processed results
    bool help_option;

    /**
     * @brief Chooses text color for output
     * @return Utils::COLOR Selected color
     */
    Utils::COLOR alternating_row_color(size_t index) const;

    /**
     * @brief Determines number of pairs to show
     * @param number_pair_found Total pairs found
     * @return int Number to actually display
     */
    int find_number_pairs_show(int number_pair_found) const;

    /**
     * @brief Formats initial results message
     * @param number_pair_found Total pairs found
     * @return string Formatted message
     */
    std::string initial_message(int found, int show) const;

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
     * @brief Prints a pair of similar paths
     * @param path1 First path in pair
     * @param path2 Second path in pair
     */
    void print_similar_path_pair(const Path& path1, const Path& path2);

    /**
     * @brief Processes a pair of similar paths
     * @param path1 First path in pair
     * @param path2 Second path in pair
     */
    void process_similar_path_pair(const Path& path1, const Path& path2);

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
     * @brief Main exploration driver
     */
    void explorer();
};
