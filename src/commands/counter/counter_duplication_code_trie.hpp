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

#pragma once

#include <map>
#include <string>
#include <vector>

#include <arkanjo/utils/utils.hpp>

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
class CounterDuplicationCodeTrie {
  private:
    static constexpr const int ZERO_INITIAL_COUNTER = 0;                     ///< Initial counter value for new nodes
    static constexpr const int ROOT_NODE = 0;                                ///< Index of root node
    static constexpr const int BASE_DEPTH = 0;                               ///< Base depth level (root)
    static constexpr const char* EMPTY_FOLDER = "";                         ///< Representation of empty folder
    static constexpr const char* BASIC_SHIFT_PER_DEPTH = "----";            ///< Indentation per depth level
    static constexpr const char* END_ARROW = "--> ";                        ///< Suffix for folder display
    static constexpr const char* TWO_POINTER_AFTER_FOLDER = "/: ";          ///< Separator after folder name
    static constexpr const char* LINE_TEXT = " duplicated lines detected."; ///< Duplication count suffix

    std::vector<std::map<std::string, int>> trie;         ///< Trie structure: {edge_value -> node}
    std::vector<int> counter_duplication_lines; ///< Duplication counts per node
    int number_printed_lines = 0;          ///< Track printed lines for formatting

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
    void create_edge_if_not_exist(int node, const std::string& folder);

    /**
     * @brief Creates indentation string based on depth
     * @param depth Current depth level
     * @return string Formatted indentation string
     */
    static std::string create_context_string_on_depth(int depth);

    /**
     * @brief Prints node information with formatting
     * @param node Node index
     * @param depth Current depth
     * @param folder Folder name to display
     */
    void print_node_information(int node, int depth, const std::string& folder);

    /**
     * @brief Performs depth-first search to print trie
     * @param current_node Starting node index
     * @param depth Current depth level
     * @param folder Current folder name
     */
    void dfs_print_duplication_code_trie(int current_node, int depth, const std::string& folder);

  public:
    /**
     * @brief Adds duplication count for a folder path
     * @param folder_path Hierarchical folder path
     * @param number_of_duplication_lines Count of duplicated lines
     */
    void add_folder_duplication_code(const std::vector<std::string>& folder_path, int number_of_duplication_lines);

    /**
     * @brief Prints the trie structure with duplication counts
     */
    void print_duplication_code_trie();

    /**
     * @brief Constructs a new trie with root node
     */
    CounterDuplicationCodeTrie();
};
