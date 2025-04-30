// Counter Duplication Code implements the relatory/counter functionality,
// where the user can see the number of duplicated lines per folder in
// the codebase

#ifndef COUNTER_DUPLICATION_CODE_HPP
#define COUNTER_DUPLICATION_CODE_HPP

#include <set>
#include "../base/path.hpp"
#include "../base/function.hpp"
#include "../base/similarity_table.hpp"
#include "counter_duplication_code_trie.hpp"
using namespace std;

class Counter_Duplication_Code{
	set<Path> processed_paths;
	Counter_Duplication_Code_Trie counter_duplication_code_trie;
	Similarity_Table *similarity_table;

	void mark_path_as_processed(Path path);
	bool is_path_processed_path(Path path);
	int get_number_of_lines_in_path(Path path);
	void register_code_duplication(Path path1, Path path2, int number_of_lines);
	void process_path(Path path);
	void process_every_path_in_similarity_table();
public:
	Counter_Duplication_Code(Similarity_Table *_similarity_table);
};

#endif 
