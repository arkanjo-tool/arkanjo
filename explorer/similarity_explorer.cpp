/*
This file expect the output of parser.cpp
This code filter the output only printing the files similar to files that contains a given pattern passed as argc
The code filter every file that has the pattern as a substring, so be carefull with duplications
*/

#include <iostream>
#include <utility>
#include <algorithm>

#include "similarity_explorer.hpp"

Utils::COLOR Similarity_Explorer::choose_text_color(){
	Utils::COLOR ret = Utils::GRAY;
	if(processed_results%2 == 0){
		ret = Utils::CYAN;
	}
	return ret;
}

int Similarity_Explorer::find_number_pairs_show(int number_pair_found) const{
	if(limit_on_results == UNLIMITED_RESULTS){
		return number_pair_found;
	}
	return std::min(limit_on_results,number_pair_found);
}

std::string Similarity_Explorer::format_initial_message(int number_pair_found){
	std::string ret;
	ret += INITIAL_TEXT_PRINT_1;
	ret += std::to_string(number_pair_found);
	ret += INITIAL_TEXT_PRINT_2;
	ret += to_string(find_number_pairs_show(number_pair_found));
	ret += INITIAL_TEXT_PRINT_3;
	return ret;
}

bool Similarity_Explorer::match_pattern(const Path& path1, const Path& path2) const{
	bool match1 = path1.contains_given_pattern(pattern_to_match);
	bool match2 = path2.contains_given_pattern(pattern_to_match);

	if(both_path_need_to_match_pattern){
		return match1 && match2;
	}
	return match1 || match2;
}

std::string Similarity_Explorer::format_path_message_in_pair(const Path& path) const{
	std::string ret = path.build_relative_path() + BETWEEN_RELATIVE_AND_FUNCTION_NAME + path.build_function_name();
	return ret;
}

int Similarity_Explorer::find_number_lines(const Path& path1) const{
	Function function(path1);
	function.load();
	return function.number_of_lines();
}

void Similarity_Explorer::print_similar_path_pair(const Path& path1, const Path& path2){
	std::string line;
	line += START_LINE_COMPARATION_PRINT;
	line += format_path_message_in_pair(path1);
	line += BETWEEN_TWO_FUNCTION;
	line += format_path_message_in_pair(path2);
	line += NUMBER_LINES_MESSAGE;
	line += std::to_string(find_number_lines(path1));

	Utils::COLOR color = choose_text_color();
	std::cout << Utils::format_colored_message(line,color) << '\n';
}

void Similarity_Explorer::process_similar_path_pair(const Path& path1, const Path& path2){
	if(!match_pattern(path1,path2)){
		return;
	}
	if(limit_on_results != UNLIMITED_RESULTS && processed_results >= limit_on_results){
		return;
	}
	processed_results++;
	print_similar_path_pair(path1,path2);
}

int Similarity_Explorer::find_number_pair_found(std::vector<std::pair<Path,Path>> similar_path_pairs) const{
	int count = 0;
	for(auto [path1, path2] : similar_path_pairs){
		if(match_pattern(path1,path2)){
			count++;
		}
	}
	return count;
}

std::vector<std::pair<Path,Path>> Similarity_Explorer::build_similar_path_pairs(bool sorted_by_number_of_duplicated_code){
	std::vector<std::pair<Path,Path>> similar_path_pairs;
	if(sorted_by_number_of_duplicated_code){
		similar_path_pairs = similarity_table->get_all_similar_path_pairs_sorted_by_line_number();
	}else{
		similar_path_pairs = similarity_table->get_all_similar_path_pairs_sorted_by_similarity();
	}
	return similar_path_pairs;
}

void Similarity_Explorer::explorer(bool sorted_by_number_of_duplicated_code){
	std::vector<std::pair<Path,Path>> similar_path_pairs = build_similar_path_pairs(sorted_by_number_of_duplicated_code);
	std::string initial_line =  format_initial_message(find_number_pair_found(similar_path_pairs));

	std::cout << initial_line << '\n';
	std::cout << Utils::LIMITER_PRINT << '\n';

	for(auto [path1, path2] : similar_path_pairs){
		process_similar_path_pair(path1,path2);
	}
}

Similarity_Explorer::Similarity_Explorer(
        Similarity_Table *_similarity_table, 
        int _limit_on_results, 
        std::string _pattern_to_match, 
        bool _both_path_need_to_match)
{
    similarity_table = _similarity_table;
    limit_on_results = _limit_on_results;
    pattern_to_match = _pattern_to_match;
    both_path_need_to_match_pattern = _both_path_need_to_match;
}

void Similarity_Explorer::run(bool sorted_by_number_of_duplicated_code){
    explorer(sorted_by_number_of_duplicated_code);
}