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

#include <arkanjo/base/path.hpp>
#include <arkanjo/base/similarity_table.hpp>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/commands/command_base.hpp>

/**
 * @brief Random pair selector for similar code segments
 *
 * Selects and displays random pairs of similar functions within
 * configurable similarity thresholds, with formatted output and
 * color-coded display options.
 */
class RandomSelector : public CommandBase<RandomSelector> {
    static constexpr const char* TEMPLATE_RANDOM_ENTRY =
      "Functions: {path_a} AND {path_b}. Similarity: {similarity} ";

    Similarity_Table* similarity_table; ///< Source of similarity data
    double minimum_similarity;          ///< Minimum similarity threshold
    double maximum_similarity;          ///< Maximum similarity threshold
    double maximum_quantity;            ///< Maximum number of pairs to show

    const int seed = 123456789;  ///< Fixed random seed for reproducibility
    std::mt19937 rng = std::mt19937(seed); ///< Mersenne Twister random generator

    /**
     * @brief Checks if pair falls within configured thresholds
     * @param path_pair Tuple of similarity and paths
     * @return bool True if pair meets criteria
     */
    bool is_valid_pair(std::tuple<double, Path, Path> path_pair);

    /**
     * @brief Gets all pairs within similarity thresholds
     * @return vector<tuple<double,Path,Path>> Filtered pairs
     */
    std::vector<std::tuple<double, Path, Path>> get_similarity_pairs_filtered();

    /**
     * @brief Performs random selection from pairs
     * @param path_pairs Full set of candidate pairs
     * @return vector<tuple<double,Path,Path>> Randomly selected subset
     */
    std::vector<std::tuple<double, Path, Path>> make_random_selection(std::vector<std::tuple<double, Path, Path>> path_pairs);

    /**
     * @brief Prints all selected path pairs
     * @param path_pairs Pairs to display
     */
    void print_path_pairs(std::vector<std::tuple<double, Path, Path>> path_pairs);

  public:
    static constexpr CliOption options_[] = {
        {"minimum_similarity", 0, PositionalArgument, nullptr},
        {"maximum_similarity", 0, PositionalArgument, nullptr},
        {"maximum_quantity", 0, PositionalArgument, nullptr},
        OPTION_END
    };
    COMMAND_DESCRIPTION(
        "Select random pairs of functions within a specified similarity range. "
        "The provided MIN_SIMILARITY and MAX_SIMILARITY parameters define the "
        "inclusive similarity interval, and up to MAX_QUANTITY pairs are randomly "
        "selected and displayed.")

    /**
     * @brief Constructs selector with configuration
     * @param _similarity_table Source of similarity data
     * @param _minimum_similarity Minimum similarity threshold (0-100)
     * @param _maximum_similarity Maximum similarity threshold (0-100)
     * @param _maximum_quantity Maximum number of pairs to select
     */
    explicit RandomSelector(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options) override;

    /**
     * @brief Handles random selection command
     */
    bool run(const ParsedOptions& options) override;
};
