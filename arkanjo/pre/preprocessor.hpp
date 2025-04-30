// Defines the preprocessor/setup step of the tool,
// where we do a heavy preprocessing of the input codebase
// to enable fast query response later.

#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

#include <string>
#include <tuple>
#include "parser.hpp"
#include "function_breaker.hpp"
#include "../base/config.hpp"
#include "duplication_finder_tool.hpp"
#include "duplication_finder_diff.hpp"
using namespace std;

class Preprocessor{
	private:
		string PROJECT_PATH_MESSAGE = "Enter your project path:";
		string MINIMUM_SIMILARITY_MESSAGE = "Enter minimum similarity desired on using the tool:";
		string CONFIG_PATH = "config.txt";
		string PATH_MESSAGE = "path of the current preprocess: ";
		string TIME_MESSAGE = "Finished time: ";

		string INITIAL_MESSAGE = "Initiating Preprocessing";
		string BREAKER_MESSAGE = "Reading codebase... (this may take a while)";
		string DUPLICATION_MESSAGE = "Finding duplication in the codebase... (this may take a while)";
		string SAVING_MESSAGE = "Saving results...";
		string END_MESSAGE = "Finished preprocessing";

		string MESSAGE_DUPLICATION_FINDER_TYPE_1 = "Enter the number of the duplication finder technique you want to use:";
		string MESSAGE_DUPLICATION_FINDER_TYPE_2 = "1) NLP text similarity using gensim";
		string MESSAGE_DUPLICATION_FINDER_TYPE_3 = "2) Count proportion of equal lines using diff command";

		string INVALID_CODE_DUPLICATION_FINDER = "Valid options are '1' or '2' only. Stopping Program...";

		tuple<string,double,bool> read_parameters();

		void save_current_run_params(string path);

		void preprocess(string path, double similarity, bool use_duplication_finder_by_tool);

	public:
		Preprocessor(bool force_preprocess);
		
		//Exposed for testing
		Preprocessor(bool force_preprocess, string path, double similarity);
};

#endif
