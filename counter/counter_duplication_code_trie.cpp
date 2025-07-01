/*
This is a Trie with a element being a folder to enable count of duplication code in the scope of files.
It is not simple to understand if you do not know the structure. Please refer to https://en.wikipedia.org/wiki/Trie .
*/
#include "counter_duplication_code_trie.hpp"

Utils::COLOR Counter_Duplication_Code_Trie::choose_text_color(){
	number_printed_lines++;
	Utils::COLOR ret = Utils::BLACK;
	if(number_printed_lines%2 == 0){
		ret = Utils::BLUE;
	}
	return ret;
}

int Counter_Duplication_Code_Trie::create_node_at_the_end(){
	int id = trie.size();
	map<string,int> aux;
	trie.push_back(aux);
	counter_duplication_lines.push_back(ZERO_INITIAL_COUNTER);
	return id;
}

void Counter_Duplication_Code_Trie::create_edge_if_not_exist(int node, string folder){
	if(trie[node].find(folder) == trie[node].end()){
		trie[node][folder] = create_node_at_the_end();
	}
}

string Counter_Duplication_Code_Trie::create_context_string_on_depth(int depth){
	string ret = "";
	for(int i = 0; i < depth; i++){
		ret += BASIC_SHIFT_PER_DEPTH;
	}
	ret += END_ARROW;
	return ret;
}


void Counter_Duplication_Code_Trie::print_node_information(int node, int depth, string folder){
	string line = create_context_string_on_depth(depth) + folder + TWO_POINTER_AFTER_FOLDER;
	line += to_string(counter_duplication_lines[node]) + LINE_TEXT;
	Utils::COLOR color = choose_text_color();
	cout << Utils::format_colored_message(line,color) << '\n';
}

void Counter_Duplication_Code_Trie::dfs_print_duplication_code_trie(int current_node, int depth, string folder){
	print_node_information(current_node,depth,folder);
	for(auto [child_folder,child_node] : trie[current_node]){
		int child_depth = depth+1;
		dfs_print_duplication_code_trie(child_node,child_depth,child_folder);
	}
}

void Counter_Duplication_Code_Trie::add_folder_duplication_code(vector<string> folder_path, int number_of_duplication_lines){
	int current_node = ROOT_NODE;
	counter_duplication_lines[current_node] += number_of_duplication_lines;
	for(auto folder : folder_path){
		create_edge_if_not_exist(current_node,folder);
		current_node = trie[current_node][folder];
		counter_duplication_lines[current_node] += number_of_duplication_lines;
	}
}

void Counter_Duplication_Code_Trie::print_duplication_code_trie(){
	dfs_print_duplication_code_trie(ROOT_NODE,BASE_DEPTH,EMPTY_FOLDER);
}

Counter_Duplication_Code_Trie::Counter_Duplication_Code_Trie(){
	create_node_at_the_end();
}
