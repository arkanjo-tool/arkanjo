/**
 * @file counter_duplication_code.hpp
 * @brief Code duplication reporting system
 *
 * Implements functionality to count and report duplicated lines
 * across folders in the codebase, providing hierarchical insights.
 * Counter Duplication Code implements the relatory/counter functionality,
 * where the user can see the number of duplicated lines per folder in
 * the codebase

 */

#ifndef COUNTER_DUPLICATION_CODE_HPP
#define COUNTER_DUPLICATION_CODE_HPP

#include <set>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/function.hpp>
#include "../base/similarity_table.hpp"

#include "counter_duplication_code_trie.hpp"
using namespace std;

/**
 * @brief Code duplication analyzer and reporter
 *
 * Provides functionality to analyze and report code duplication metrics
 * across the codebase, organized by folder hierarchy using a trie structure
 * for efficient hierarchical counting and reporting.
 */
class Counter_Duplication_Code {
        private:
                set<Path> processed_paths;                      ///< Tracks processed paths to avoid duplicates
                Counter_Duplication_Code_Trie counter_duplication_code_trie;  ///< Trie for hierarchical counting
                Similarity_Table *similarity_table;             ///< Similarity data source

                /**
                 * @brief Marks a path as processed
                 * @param path Path to mark
                 */
                void mark_path_as_processed(Path path);

                /**
                 * @brief Checks if path was already processed
                 * @param path Path to check
                 * @return bool True if path was processed
                 */
                bool is_path_processed_path(Path path);

                /**
                 * @brief Gets line count for a path
                 * @param path Path to analyze
                 * @return int Number of lines in the file
                 */
                int get_number_of_lines_in_path(Path path);

                /**
                 * @brief Registers a duplication between two paths
                 * @param path1 First path in duplication pair
                 * @param path2 Second path in duplication pair
                 * @param number_of_lines Number of duplicated lines
                 */
                void register_code_duplication(Path path1, Path path2, int number_of_lines);

                /**
                 * @brief Processes a single path for duplication
                 * @param path Path to process
                 */
                void process_path(Path path);

                /**
                 * @brief Processes all paths in similarity table
                 */
                void process_every_path_in_similarity_table();

        public:
                /**
                 * @brief Constructs analyzer with similarity data
                 * @param _similarity_table Pointer to similarity table
                 */
                Counter_Duplication_Code(Similarity_Table *_similarity_table);
};

#endif