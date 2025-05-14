// Defines the preprocessor/setup step of the tool,
// where we do a heavy preprocessing of the input codebase
// to enable fast query response later.

#ifndef DUPLICATION_FINDER_TOOL_HPP 
#define DUPLICATION_FINDER_TOOL_HPP 

#include <string>
#include <iostream>
#include "parser.hpp"
using namespace std;

class DuplicationFinderTool{
	private:
		string SAVING_MESSAGE = "Saving results...";

		string base_path;
		double similarity;
	public:
		DuplicationFinderTool(string base_path_, double similatiry_);
		void execute();
};

#endif
