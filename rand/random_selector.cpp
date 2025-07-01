#include "random_selector.hpp"

Utils::COLOR Random_Selector::choose_text_color(){
	Utils::COLOR ret = Utils::GRAY;
	if(processed_results%2 == 0){
		ret = Utils::CYAN;
	}
	return ret;
}

string Random_Selector::format_path_message_in_pair(Path path){
	string ret = path.build_relative_path() + BETWEEN_RELATIVE_AND_FUNCTION_NAME + path.build_function_name();
	return ret;
}

bool Random_Selector::is_valid_pair(tuple<double,Path,Path> path_pair){
	auto [similarity,path1,path2] = path_pair;
	if(similarity < minimum_similarity) return false;
	if(similarity > maximum_similarity) return false;
	return true;
}

vector<tuple<double,Path,Path>> Random_Selector::get_similarity_pairs_filtered(){
	auto path_pairs = similarity_table->get_all_path_pairs_and_similarity_sorted_by_similarity();
	vector<tuple<double,Path,Path>> ret;
	for(auto path_pair : path_pairs){
		if(is_valid_pair(path_pair)){
			ret.push_back(path_pair);
		}
	}
	return ret;
}

vector<tuple<double,Path,Path>> Random_Selector::make_random_selection(vector<tuple<double,Path,Path>> path_pairs){
	shuffle(path_pairs.begin(),path_pairs.end(), rng);
	while( int(path_pairs.size()) > maximum_quantity){
		path_pairs.pop_back();
	}
	return path_pairs;
}

void Random_Selector::print_path_pair(tuple<double,Path,Path> path_pair){
	auto [similarity,path1,path2] = path_pair;
	string line;
	line += START_LINE_COMPARATION_PRINT;
	line += format_path_message_in_pair(path1);
	line += BETWEEN_TWO_FUNCTION;
	line += format_path_message_in_pair(path2);
	line += SIMILARITY_MESSAGE;
	line += to_string(similarity);
	Utils::COLOR color = choose_text_color();
	cout << Utils::format_colored_message(line,color) << '\n';
}

void Random_Selector::print_path_pairs(vector<tuple<double,Path,Path>> path_pairs){
	for(auto path_pair : path_pairs){
		print_path_pair(path_pair);
	}
}

Random_Selector::Random_Selector(
		Similarity_Table *_similarity_table,
		double _minimum_similarity,
		double _maximum_similarity,
		double _maximum_quantity
		){
	similarity_table = _similarity_table;
	similarity_table->update_similarity(0);
	minimum_similarity = _minimum_similarity;
	maximum_similarity = _maximum_similarity;
	maximum_quantity = _maximum_quantity;

	auto path_pairs = get_similarity_pairs_filtered();
	path_pairs = make_random_selection(path_pairs);
	print_path_pairs(path_pairs);
}
