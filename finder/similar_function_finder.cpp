#include "similar_function_finder.hpp"

void Similar_Function_Finder::find_path_that_meets_pattern(){
	vector<Path> paths = similarity_table->get_path_list();
	for(auto _path : paths){
		if(_path.contains_given_pattern(function_pattern)){
			path = _path;
		}
	}
}

void Similar_Function_Finder::print_empty_path_message(){
	string line1 = EMPTY_PATH_MESSAGE_1 + function_pattern;
	string line2 = EMPTY_PATH_MESSAGE_2;
	cout << line1 << '\n';
	cout << line2 << '\n';
}

void Similar_Function_Finder::print_function(Path path){
	Function function(path);
	function.print_basic_info();
}

void Similar_Function_Finder::print_reference_path(){
	string line1 = REFERENCE_PATH_MESSAGE;
	cout << line1 << '\n';
	print_function(path);
}

void Similar_Function_Finder::print_similar_functions(vector<Path> similar_paths){
	string line1 = COUNT_MESSAGE_1 + to_string(similar_paths.size()) + COUNT_MESSAGE_2;
	cout << line1 << '\n';
	for(auto similar_path : similar_paths){
		print_function(similar_path);
	}
}

void Similar_Function_Finder::print_similar_functions(){
	if(path.is_empty()){
		print_empty_path_message();
		return;
	}
	vector<Path> similar_paths = similarity_table->get_similar_path_to_the_reference(path);

	cout << Utils::LIMITER_PRINT << '\n';
	print_reference_path();
	print_similar_functions(similar_paths);
}

Similar_Function_Finder::Similar_Function_Finder(string _function_pattern, Similarity_Table *_similarity_table){
	similarity_table = _similarity_table;
	function_pattern = _function_pattern;

	find_path_that_meets_pattern();
	print_similar_functions();
}
