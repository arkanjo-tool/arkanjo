// Defines util functions, struct and constants used across 
// the function breaker process for the suported
// languages

#ifndef FUNCTION_BREAKER_UTIL_HPP
#define FUNCTION_BREAKER_UTIL_HPP

#include <string>
#include <vector>
#include "../base/utils.hpp"
#include "../base/config.hpp"

struct Line_content{
	int line_number;
	string content;
};

enum PROGRAMMING_LANGUAGE{
	C,
	JAVA
};


const string SOURCE_PATH = "source";
const string HEADER_PATH = "header";
const string INFO_PATH =   "info";

string extract_extension(string file_path);

string build_source_path(string relative_path, string function_name);

string build_header_path(string relative_path, string function_name);

string build_info_path(string relative_path, string function_name);

void create_source_file(int start_number_line, int end_number_line, string relative_path, string function_name, const vector<string> &function_content);

void create_header_file(string relative_path, string function_name, const vector<string> &header_content);

/*This creates a json file*/
void create_info_file(int line_declaration, int start_number_line, int end_number_line, string relative_path, string function_name);

#endif
