// Defines the flow of parsing a java source file in multiples functions

#ifndef FUNCTION_BREAKER_JAVA_HPP
#define FUNCTION_BREAKER_JAVA_HPP

#include <string>
#include <vector>
#include <set>
#include <array>
#include <utility>
#include "../base/utils.hpp"
#include "function_breaker_util.hpp"

class FunctionBreakerJava{

	const int NUMBER_OF_LINES_BEFORE_FOR_FUNCTION_NAME = 7;
	const int C_RELEVANT_DEPTH = 0;
	const int JAVA_RELEVANT_DEPTH = 1;
	const bool IGNORE_EMPTY_FUNCTIONS = true;

	const bool ALLOW_STRUCTS = false;

	set<array<int,3>> find_start_end_and_depth_of_brackets(vector<string> brackets_content);

	set<pair<int,int>> find_start_end_of_brackets_of_given_depth(vector<string> brackets_content, int depth);

	int find_position_first_open_bracket(string s);

	string extract_last_token_of_string(string s);

	Line_content build_line_code(int line_number, string content);

	vector<Line_content> get_lines_before_body_function(const vector<string> &file_content, int line_start_body_function, int pos_bracket);

	vector<Line_content> remove_parenteses_at_the_end_of_the_scope(vector<Line_content> code);

	vector<Line_content> remove_content_until_find_parenteses_at_the_end(vector<Line_content> code);

	vector<Line_content> remove_parameters_of_declaration_c(vector<Line_content> code);

	vector<Line_content> remove_parameters_of_declaration_java(vector<Line_content> code);

	vector<Line_content> remove_parameters_of_declaration(vector<Line_content> code, PROGRAMMING_LANGUAGE programming_language);

	pair<string,int> extract_function_name_and_line_from_declaration(const vector<string> &file_content, int line_start_body_function, PROGRAMMING_LANGUAGE programming_language);

	vector<string> build_function_content(int start_number_line, int end_number_line,const vector<string> &file_content);

	vector<string> build_header_content(int start_number_line, int line_declaration, string relative_path, string function_name, const vector<string> &file_content);

	bool is_body_function_empty(int start_number_line, int end_number_line,const vector<string> &file_content);

	void process_function(int start_number_line, int end_number_line, string relative_path, const vector<string> &file_content, PROGRAMMING_LANGUAGE programming_language);

	string file_path_from_folder_path(string file_path, string folder_path);

	void file_breaker_java(string file_path, string folder_path);
public:
	FunctionBreakerJava(string file_path, string folder_path);

};

#endif
