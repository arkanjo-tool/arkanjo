// Similar Functions Finder implements the functionality to
// given a function, find the other functions that are similar
// to the given one.

#ifndef SIMILAR_FUNCTION_FINDER_HPP
#define SIMILAR_FUNCTION_FINDER_HPP

#include <string>
#include <vector>

#include "../base/path.hpp"
#include "../base/function.hpp"
#include "../base/utils.hpp"
#include "../base/similarity_table.hpp"
using namespace std;

class Similar_Function_Finder{
	string EMPTY_PATH_MESSAGE_1 = "There is no functions that resembles the name: ";
	string EMPTY_PATH_MESSAGE_2 = "Value incorrected passed or there is no duplication code with this function";
	string REFERENCE_PATH_MESSAGE = "The following function was found:";
	string COUNT_MESSAGE_1 = "The total number of functions that are similar to the found one is ";
	string COUNT_MESSAGE_2 = ". More info about them are listed below.";

	Similarity_Table *similarity_table;
	string function_pattern;
	Path path;

	void find_path_that_meets_pattern();
	void print_empty_path_message();
	void print_function(Path path);
	void print_reference_path();
	void print_similar_functions(vector<Path> similar_paths);
	void print_similar_functions();

public:
	Similar_Function_Finder(string _function_pattern, Similarity_Table *_similarity_table);
};

#endif
