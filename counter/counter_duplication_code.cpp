#include "counter_duplication_code.hpp"

void Counter_Duplication_Code::mark_path_as_processed(Path path){
	processed_paths.insert(path);
}

bool Counter_Duplication_Code::is_path_processed_path(Path path){
	return processed_paths.find(path) != processed_paths.end();
}

int Counter_Duplication_Code::get_number_of_lines_in_path(Path path){
	Function function(path);
	return function.number_of_lines();
}

void Counter_Duplication_Code::register_code_duplication(Path path1, Path path2, int number_of_lines){
	vector<string> common_folder_path = path1.get_common_folders(path2);
	counter_duplication_code_trie.add_folder_duplication_code(common_folder_path, number_of_lines);
}

void Counter_Duplication_Code::process_path(Path path){
	if(is_path_processed_path(path)){
		return;
	}
	int number_of_lines = get_number_of_lines_in_path(path);
	mark_path_as_processed(path);
	vector<Path> similar_paths = similarity_table->get_similar_path_to_the_reference(path);
	for(auto similar_path : similar_paths){
		mark_path_as_processed(similar_path);
		register_code_duplication(path, similar_path, number_of_lines);
	}
}

void Counter_Duplication_Code::process_every_path_in_similarity_table(){
	vector<Path> paths = similarity_table->get_path_list();
	for(auto path : paths){
		process_path(path);
	}
}

Counter_Duplication_Code::Counter_Duplication_Code(Similarity_Table *_similarity_table){
	similarity_table = _similarity_table;
	process_every_path_in_similarity_table();
	counter_duplication_code_trie.print_duplication_code_trie();
}
