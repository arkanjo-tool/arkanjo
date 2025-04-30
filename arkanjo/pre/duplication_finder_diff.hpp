// Defines the preprocessor/setup step of the tool,
// where we do a heavy preprocessing of the input codebase
// to enable fast query response later.

#ifndef DUPLICATION_FINDER_DIFF_HPP 
#define DUPLICATION_FINDER_DIFF_HPP 

#include <string>
#include <vector>
#include <tuple>
#include "../base/utils.hpp"
using namespace std;

class DuplicationFinderDiff{
	private:
		string SAVING_MESSAGE = "Saving results...";

		string base_path;
		double similarity;

		vector<string> find_files(string folder_path);
		bool is_empty_line(string line);
		vector<string> remove_blank_lines(vector<string> content);
		bool is_equal_files(vector<string> content1, vector<string> content2);
		double find_similarity(string path1, string path2);
		vector<tuple<double,string,string>> find_similar_pairs(vector<string> &file_paths);
		void save_duplications(vector<tuple<double,string,string>> &file_duplication_pairs);
	public:
		DuplicationFinderDiff(string base_path_, double similatiry_);
		void execute();
};

#endif
