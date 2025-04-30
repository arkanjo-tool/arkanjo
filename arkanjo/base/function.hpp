// This is the abstraction of a function in the temporary codebase, 
// contains all the information related to a function, and for that 
// open and read the files in the temporary codebase.

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <vector>
#include <array>
#include <iostream>

//WARNING THIS IMPORT MAYBE ONLY WORKS IN UBUNTU, SEE HERE: https://github.com/stardust95/TinyCompiler/issues/2
#include <json/json.h>

#include "path.hpp"
#include "utils.hpp"
using namespace std;

class Function{
	private:
		string LINE_DECLARATION_JSON = "line_declaration";
		string START_NUMBER_LINE_JSON = "start_number_line";
		string END_NUMBER_LINE_JSON = "end_number_line";

		string FUNCTION_PREFIX_PRINT = "Function Name: ";
		string RELATIVE_PATH_PRINT = "Relative Path: ";
		string LINE_DECLARATION_PRINT = "Starts on line: ";
		string END_DECLARATION_PRINT = "Ends on line: ";
		string NUMBER_LINE_PRINT = "Total number of lines: ";

		Path path;
		vector<string> content;
		vector<string> header;
		int start_number_line;
		int line_declaration;
		int end_number_line;

		void read_content();
		void read_header();
		void read_info();
	public:
		int number_of_lines();
		Function(Path _path);
		array<int,3> get_scope_function_in_file();
		vector<string> get_header();
		vector<string> build_all_content();
		void print_basic_info();
};

#endif
