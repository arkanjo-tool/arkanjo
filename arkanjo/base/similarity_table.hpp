/**
 * @file similarity_table.hpp
 * @brief Similarity relationships storage and analysis
 *
 * Stores and analyzes similarity relationships between code functions,
 * including similarity probabilities and threshold-based filtering.
 * 
 * Similarity Table is a abstraction that store the pair of functions 
 * that are similar to each other and the similarity probability between
 * them.
 */

#ifndef SIMILARITY_TABLE_HPP
#define SIMILARITY_TABLE_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <tuple>
#include <fstream>

#include "path.hpp"
#include "utils.hpp"
#include "function.hpp"
using namespace std;

/**
 * @brief Manages and analyzes function similarity relationships
 *
 * Stores pairs of similar functions with their similarity scores,
 * providing various query and analysis capabilities.
 */
class Similarity_Table {
        private:
                string SIMILARITY_TABLE_FILE_NAME = "tmp/output_parsed.txt";  ///< Default similarity data file
                double DEFAULT_SIMILARITY = 100.00;                         ///< Default similarity threshold
                double EPS_ERROR_MARGIN = 1e-6;                             ///< Floating-point comparison margin
                double MAXIMUM_SIMILARITY = 100.00;                         ///< Maximum possible similarity score
                double MINIMUM_SIMILARITY = 0.00;                           ///< Minimum possible similarity score

                double similarity_threshold;                                ///< Current similarity threshold
                vector<Path> paths;                                        ///< List of all known paths
                map<Path,int> path_id;                                     ///< Path to ID mapping
                vector<vector<pair<int,double>>> similarity_graph;         ///< Graph of similarity relationships
                map<pair<int,int>,double> similarity_table;                ///< Similarity score lookup table

                /**
                 * @brief Finds or assigns ID for a path
                 * @param path Path to look up
                 * @return int Unique ID for the path
                 */
                int find_id_path(Path path);

                /**
                 * @brief Reads single comparison from file
                 * @param table_file Input file stream
                 */
                void read_comparation(ifstream &table_file);

                /**
                 * @brief Reads entire similarity table from file
                 * @param table_file Input file stream
                 */
                void read_file_table(ifstream &table_file);

                /**
                 * @brief Initializes similarity table from file
                 */
                void init_similarity_table();

                /**
                 * @brief Checks if similarity meets threshold
                 * @param similarity Similarity score to check
                 * @return bool True if score meets threshold
                 */
                bool is_above_threshold(double similarity);

                /**
                 * @brief Sorts path pairs by line count
                 * @param similar_path_pairs Pairs to sort
                 * @return Sorted pairs with line counts
                 */
                vector<tuple<int,Path,Path>> sort_pairs_by_line_number(vector<pair<Path,Path>> similar_path_pairs);

        public:
                /**
                 * @brief Constructs with custom similarity threshold
                 * @param _similarity_threshold Initial threshold value
                 */
                Similarity_Table(double _similarity_threshold);
                
                /**
                 * @brief Constructs with default similarity threshold
                 */
                Similarity_Table();
                
                /**
                 * @brief Updates similarity threshold
                 * @param new_similarity_threshold New threshold value
                 */
                void update_similarity(double new_similarity_threshold);

                /**
                 * @brief Gets similarity between two paths
                 * @param path1 First path to compare
                 * @param path2 Second path to compare
                 * @return double Similarity score
                 */
                double get_similarity(Path path1, Path path2);

                /**
                 * @brief Checks if two paths are similar
                 * @param path1 First path to compare
                 * @param path2 Second path to compare
                 * @return bool True if paths are similar
                 */
                double is_similar(Path path1, Path path2);

                /**
                 * @brief Gets list of all known paths
                 * @return vector<Path> All paths in table
                 */
                vector<Path> get_path_list();

                /**
                 * @brief Gets paths similar to reference path
                 * @param reference Path to compare against
                 * @return vector<Path> Similar paths
                 */
                vector<Path> get_similar_path_to_the_reference(Path reference);

                /**
                 * @brief Gets all similar path pairs with scores, sorted
                 * @return vector<tuple<double,Path,Path>> Similar pairs with scores
                 */
                vector<tuple<double,Path,Path>> get_all_path_pairs_and_similarity_sorted_by_similarity();

                /**
                 * @brief Gets all similar path pairs, sorted by similarity
                 * @return vector<pair<Path,Path>> Similar path pairs
                 */
                vector<pair<Path,Path>> get_all_similar_path_pairs_sorted_by_similarity();

                /**
                 * @brief Gets all similar path pairs, sorted by line count
                 * @return vector<pair<Path,Path>> Similar path pairs
                 */
                vector<pair<Path,Path>> get_all_similar_path_pairs_sorted_by_line_number();
};

#endif