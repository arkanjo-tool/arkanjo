/**
 * @file duplication_finder_diff.hpp
 * @brief Code duplication preprocessing system
 *
 * Implements the heavy preprocessing stage that analyzes the codebase
 * for potential duplications to enable fast query responses later.
 * 
 * Defines the preprocessor/setup step of the tool,
 * where we do a heavy preprocessing of the input codebase
 * to enable fast query response later.
 */

#ifndef DUPLICATION_FINDER_DIFF_HPP
#define DUPLICATION_FINDER_DIFF_HPP

#include <string>
#include <vector>
#include <tuple>
#include "../base/utils.hpp"
using namespace std;

/**
 * @brief Code duplication preprocessor
 *
 * Performs initial analysis of the codebase to identify and catalog
 * potential code duplications, creating a foundation for fast similarity
 * queries during the main operation phase.
 */
class DuplicationFinderDiff {
        private:
                string SAVING_MESSAGE = "Saving results...";  ///< Status message for saving output

                string base_path;  ///< Root path of the codebase to analyze
                double similarity;  ///< Similarity threshold for considering duplicates

                /**
                 * @brief Finds all files in a directory tree
                 * @param folder_path Root path to search from
                 * @return vector<string> List of file paths found
                 */
                vector<string> find_files(string folder_path);

                /**
                 * @brief Checks if a line is empty/whitespace
                 * @param line Line to check
                 * @return bool True if line is empty
                 */
                bool is_empty_line(string line);

                /**
                 * @brief Removes blank lines from file content
                 * @param content Original file content
                 * @return vector<string> Content without blank lines
                 */
                vector<string> remove_blank_lines(vector<string> content);

                /**
                 * @brief Compares two files for exact equality
                 * @param content1 First file's content
                 * @param content2 Second file's content
                 * @return bool True if files are identical
                 */
                bool is_equal_files(vector<string> content1, vector<string> content2);

                /**
                 * @brief Calculates similarity between two files
                 * @param path1 Path to first file
                 * @param path2 Path to second file
                 * @return double Similarity score (0-100)
                 */
                double find_similarity(string path1, string path2);

                /**
                 * @brief Finds all similar file pairs above threshold
                 * @param file_paths List of files to compare
                 * @return vector<tuple<double,string,string>> Similar pairs with scores
                 */
                vector<tuple<double,string,string>> find_similar_pairs(vector<string> &file_paths);

                /**
                 * @brief Saves duplication results to output
                 * @param file_duplication_pairs Pairs to save
                 */
                void save_duplications(vector<tuple<double,string,string>> &file_duplication_pairs);

        public:
                /**
                 * @brief Constructs preprocessor with configuration
                 * @param base_path_ Root path of codebase
                 * @param similarity_ Similarity threshold (0-100)
                 */
                DuplicationFinderDiff(string base_path_, double similarity_);

                /**
                 * @brief Executes the preprocessing pipeline
                 */
                void execute();
};

#endif