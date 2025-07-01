#include "big_clone_formater.hpp"

string Big_Clone_Formater::format_relative_path(string relative_path){
	for(auto &c : relative_path){
		if(c == '/'){
			c = ',';
		}
	}
	return relative_path;		
}

string Big_Clone_Formater::build_path_formated_string(Path path){
	string relative_path = path.build_relative_path();
	relative_path = format_relative_path(relative_path);

	Function function(path);
	auto [line_declaration,start_line,end_line] = function.get_scope_function_in_file();

	string ret;
	ret += relative_path;
	ret += ',' + to_string(line_declaration);
	ret += ',' + to_string(start_line);
	ret += ',' + to_string(end_line);

	return ret;
}
auto Big_Clone_Formater::process_similar_path_pair(Path path1, Path path2, double similarity){
	auto string_path1 = build_path_formated_string(path1);
	auto string_path2 = build_path_formated_string(path2);

	string comparation_string = string_path1 + ',' + string_path2 + ',';
	cout << comparation_string;
	cout << fixed << setprecision(2) << similarity << '\n';
}

Big_Clone_Formater::Big_Clone_Formater(Similarity_Table *similarity_table){
	auto similar_paths = similarity_table->get_all_path_pairs_and_similarity_sorted_by_similarity();
	for(auto [similarity,path1,path2] : similar_paths){
		process_similar_path_pair(path1,path2,similarity);
	}
}
