/**
 * @file big_clone_tailor_evaluator.hpp
 * @brief BigCloneBench evaluation interface *
 * Provides evaluation metrics compatible with BigCloneBench dataset
 * using methodology similar to the Tailor tool for clone detection.
 *
 * This class is used to evaluate the tool by the big clone bench dataset
 * using the same methodology as the Tailor tool.
 */

#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include <arkanjo/base/path.hpp>
#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/utils/utils.hpp>

#include <arkanjo/commands/command_base.hpp>

using namespace std;

/**
 * @brief Evaluates clone detection using BigCloneBench methodology
 *
 * Implements evaluation metrics that compare detected clones against
 * BigCloneBench ground truth data, following Tailor tool's approach.
 */
class BigCloneTailorEvaluator : public CommandBase<BigCloneTailorEvaluator> {
  private:
    string CLONE_LABELS_FILE_PATH = "/home/luan/linux/test-duplication-code/parser/bigclonebench/clone_labels.txt"; ///< Path to ground truth labels
    string EXTENSION = ".java";                                                                                     ///< Expected file extension
    int NUMBER_OF_TYPES = 6;                                                                                        ///< Number of clone types
    int NOT_CLONE_TYPE_ID = 5;                                                                                      ///< ID for non-clone pairs
    string RECALL_PER_TYPE_PRINT = "Recall Value of each type";                                                     ///< Recall header text
    double MINIMUM_SIMILARITY_TEMP = 0;                                                                             ///< Temporary similarity threshold

    /// Clone type labels for output
    vector<string> ID_TO_TYPE_LABEL = {
        "T1",
        "T2",
        "ST3",
        "MT3",
        "WT3/T4",
        "Negative",
    };

    Similarity_Table* similarity_table;       ///< Pointer to similarity data
    map<pair<int, int>, int> id_pair_to_type; ///< Maps ID pairs to clone types
    vector<int> count_of_samples_by_type;     ///< Count of samples per type

    /**
     * @brief Reads clone labels from ground truth file
     */
    void read_clone_labels();

    /**
     * @brief Converts Path to numeric ID
     * @param path Path to convert
     * @return int Extracted numeric ID
     */
    int path_to_id(Path path);

    /**
     * @brief Formats similar path pairs with IDs
     * @return vector<tuple<double,int,int>> Similarity scores with ID pairs
     */
    vector<tuple<double, int, int>> similar_path_pairs_formated_with_id();

    /**
     * @brief Checks if ID pair exists in ground truth
     * @param id0 First ID in pair
     * @param id1 Second ID in pair
     * @return bool True if pair is in ground truth
     */
    bool is_relevant_pair(int id0, int id1);

    /**
     * @brief Filters ID pairs to only relevant ones
     * @param similar_id_pairs Pairs to filter
     * @return set<pair<int,int>> Filtered pairs
     */
    set<pair<int, int>> filter_similar_id_pairs_only_relevant_ones(
        vector<pair<int, int>> similar_id_pairs);

    /**
     * @brief Filters pairs by similarity threshold
     * @param similar_id_pairs Pairs to filter
     * @param minimum_similarity Threshold value
     * @return vector<pair<int,int>> Filtered pairs
     */
    vector<pair<int, int>> filter_similar_path_pairs_by_similarity(
        vector<tuple<double, int, int>> similar_id_pairs,
        double minimum_similarity);

    /**
     * @brief Builds frequency statistics per clone type
     * @param similar_id_pairs Pairs to analyze
     * @return vector<int> Frequency counts per type
     */
    vector<int> build_frequency_corrected_guessed_by_type(
        vector<pair<int, int>> similar_id_pairs);

    /**
     * @brief Calculates recall for specific clone type
     * @param frequency Type frequencies
     * @param type Clone type to evaluate
     * @return double Recall value
     */
    double calc_recall(vector<int> frequency, int type);

    /**
     * @brief Prints recall metrics per type
     * @param frequency Type frequencies
     */
    void print_recall_per_type(vector<int> frequency);

    /**
     * @brief Runs full evaluation pipeline
     * @param minimum_similarity Similarity threshold
     */
    void evaluate(double minimum_similarity);

  public:
    COMMAND_DESCRIPTION(
        "Evaluate clone detection results using the BigCloneBench dataset "
        "methodology. The command compares detected similarities against "
        "ground truth labels and reports recall metrics per clone type.")

    /**
     * @brief Constructs evaluator with similarity data
     * @param _similarity_table Pointer to similarity table
     */
    explicit BigCloneTailorEvaluator(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options);

    /**
     * @brief Handles BigCloneBench evaluation command
     */
    bool run(const ParsedOptions& options) override;
};
