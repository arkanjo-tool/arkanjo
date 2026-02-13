/**
 * @file counter_duplication_code_trie.hpp
 * @brief Trie-based code duplication counter
 *
 * Implements a trie structure where nodes represent folders to enable
 * hierarchical counting of duplicated lines across the codebase.
 * Implements a TRIE where the nodes are folders, this enable
 * the project to count the number of duplicated lines per folde
 * in the codebase. 

 */

#ifndef COUNTER_DUPLICATION_CODE_TRIE_HPP
#define COUNTER_DUPLICATION_CODE_TRIE_HPP

#include <string>
#include <vector>
#include <map>

#include "arkanjo/base/utils.hpp"
using namespace std;

/**
 * @brief Trie structure for hierarchical duplication counting
 *
 * Implements a modified trie where each node represents a folder in the
 * codebase, enabling efficient counting and reporting of duplicated lines
 * at different directory levels.
 *
 * @note For understanding the trie structure, refer to: 
 *       https://en.wikipedia.org/wiki/Trie
 */
class Counter_Duplication_Code_Trie {
        private:
                int ZERO_INITIAL_COUNTER = 0;                ///< Initial counter value for new nodes
                int ROOT_NODE = 0;                           ///< Index of root node
                int BASE_DEPTH = 0;                          ///< Base depth level (root)
                string EMPTY_FOLDER = "";                    ///< Representation of empty folder
                string BASIC_SHIFT_PER_DEPTH = "----";       ///< Indentation per depth level
                string END_ARROW = "--> ";                   ///< Suffix for folder display
                string TWO_POINTER_AFTER_FOLDER = "/: ";     ///< Separator after folder name
                string LINE_TEXT = " duplicated lines detected."; ///< Duplication count suffix

                vector<map<string,int>> trie;                ///< Trie structure: {edge_value -> node}
                vector<int> counter_duplication_lines;       ///< Duplication counts per node
                int number_printed_lines = 0;                ///< Track printed lines for formatting

                /**
                 * @brief Chooses text color based on duplication count
                 * @return COLOR Text color for output
                 */
                Utils::COLOR choose_text_color();

                /**
                 * @brief Creates new node at end of trie
                 * @return int Index of new node
                 */
                int create_node_at_the_end();

                /**
                 * @brief Creates edge if it doesn't exist
                 * @param node Source node index
                 * @param folder Folder name (edge value)
                 */
                void create_edge_if_not_exist(int node, string folder);

                /**
                 * @brief Creates indentation string based on depth
                 * @param depth Current depth level
                 * @return string Formatted indentation string
                 */
                string create_context_string_on_depth(int depth);

                /**
                 * @brief Prints node information with formatting
                 * @param node Node index
                 * @param depth Current depth
                 * @param folder Folder name to display
                 */
                void print_node_information(int node, int depth, string folder);

                /**
                 * @brief Performs depth-first search to print trie
                 * @param current_node Starting node index
                 * @param depth Current depth level
                 * @param folder Current folder name
                 */
                void dfs_print_duplication_code_trie(int current_node, int depth, string folder);

        public:
                /**
                 * @brief Adds duplication count for a folder path
                 * @param folder_path Hierarchical folder path
                 * @param number_of_duplication_lines Count of duplicated lines
                 */
                void add_folder_duplication_code(vector<string> folder_path, int number_of_duplication_lines);

                /**
                 * @brief Prints the trie structure with duplication counts
                 */
                void print_duplication_code_trie();

                /**
                 * @brief Constructs a new trie with root node
                 */
                Counter_Duplication_Code_Trie();
};

#endif