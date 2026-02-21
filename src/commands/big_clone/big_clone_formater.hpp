/**
 * @file big_clone_formater.hpp
 * @brief BigCloneEval format conversion interface
 *
 * Provides functionality to format clone detection results into the
 * BigCloneBench dataset compatible output format for evaluation.
 * This files is related to the first try to evaluate the tool in big clone bench dataset
 * the class does not work properly in the moment and should be ignored.
 */

#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/similarity_table.hpp>

#include <arkanjo/commands/command_base.hpp>

using namespace std;

/**
 * @brief Formats clone detection results for BigCloneBench evaluation
 *
 * Converts internal path and similarity representations into the
 * standardized BigCloneEval output format expected by the BigCloneBench
 * dataset evaluation tools.
 *
 * @note Designed to operate within the BigCloneBench dataset directory structure
 */
class BigCloneFormater : public CommandBase<BigCloneFormater> {
  private:
    Similarity_Table* similarity_table; ///< Similarity data source

    /**
     * @brief Formats relative path string for BigCloneEval output
     * @param relative_path Path string to format
     * @return string Path with separators converted to commas
     */
    string format_relative_path(string relative_path);

    /**
     * @brief Builds complete formatted path string with line numbers
     * @param path Path object to format
     * @return string Formatted path in "path,decl,start,end" format
     */
    string build_path_formated_string(Path path);

    /**
     * @brief Processes and outputs a similar path pair
     * @param path1 First path in pair
     * @param path2 Second path in pair
     * @param similarity Similarity score between paths
     */
    auto process_similar_path_pair(Path path1, Path path2, double similarity);

  public:
    COMMAND_DESCRIPTION(
        "Format clone detection results into the BigCloneBench evaluation "
        "output format. The command converts internal similarity data into "
        "the standardized BigCloneEval compatible representation.")

    /**
     * @brief Constructs formatter and processes similarity table
     * @param similarity_table Pointer to similarity data to format
     *
     * @note Immediately outputs all formatted pairs to stdout
     */
    explicit BigCloneFormater(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options);

    /**
     * @brief Handles BigCloneEval formatting command
     */
    bool run(const ParsedOptions& options) override;
};