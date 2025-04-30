// Implements a TRIE where the nodes are folders, this enable
// the project to count the number of duplicated lines per folder
// in the codebase. 

#ifndef COUNTER_DUPLICATION_CODE_TRIE_HPP
#define COUNTER_DUPLICATION_CODE_TRIE_HPP

#include <string>
#include <vector>
#include <map>

#include "../base/utils.hpp"
using namespace std;

/*
This is a Trie with a element being a folder to enable count of duplication code in the scope of files.
It is not simple to understand if you do not know the structure. Please refer to https://en.wikipedia.org/wiki/Trie .
*/
class Counter_Duplication_Code_Trie{
	int ZERO_INITIAL_COUNTER = 0;
	int ROOT_NODE = 0;
	int BASE_DEPTH = 0;
	string EMPTY_FOLDER = "";
	string BASIC_SHIFT_PER_DEPTH = "----";
	string END_ARROW = "--> ";
	string TWO_POINTER_AFTER_FOLDER = "/: ";
	string LINE_TEXT = " duplicated lines detected."; 

	vector<map<string,int>> trie; //It is a pair {value of the edge which is a string, node that this edges moves to}
	vector<int> counter_duplication_lines;
	int number_printed_lines = 0;

	Utils::COLOR choose_text_color();
	int create_node_at_the_end();
	void create_edge_if_not_exist(int node, string folder);
	string create_context_string_on_depth(int depth);
	void print_node_information(int node, int depth, string folder);
	void dfs_print_duplication_code_trie(int current_node, int depth, string folder);

public:
	void add_folder_duplication_code(vector<string> folder_path, int number_of_duplication_lines);
	void print_duplication_code_trie();
	Counter_Duplication_Code_Trie();
};

#endif
