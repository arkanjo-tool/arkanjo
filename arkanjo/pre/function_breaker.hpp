/*
This file intends to create the process of reading a directory with 
code in c or java and extract every function/struct of every .c /.java file as a new file
in the temporary repository, aka, tmp/

Example of expected behaviour: 
There is a file example.c with functions a and b, will be create two new 
files: example/a.c and example/b.c

TODO: Doesn't work correct if the file has an incorrect bracket sequence, 
even if the bracket sequence is in a commentary
*/

#ifndef FUNCTION_BREAKER_HPP
#define FUNCTION_BREAKER_HPP

#include <string>
#include <vector>
#include "../base/utils.hpp"
#include "function_breaker_c.hpp"
#include "function_breaker_java.hpp"
#include "function_breaker_util.hpp"
using namespace std;

class FunctionBreaker{

	const vector<string> C_EXTENSIONS = { "c","h"};
	const vector<string> JAVA_EXTENSIONS =  { "java" };
	const vector<string> ALLOWED_EXTENSIONS = { "c","h","java" };

	bool is_c_extension(string extension);

	bool is_java_extension(string extension);

	bool is_allowed_extension(string extension);


	void file_breaker(string file_path, string folder_path);

	void function_breaker(string folder_path);

	public:
	FunctionBreaker(string folder_path);
};

#endif
