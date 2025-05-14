// The Random Selector random selects a set of duplicated functions pairs
// enables to set the interval of acceptable similarity probability and 
// the number of functions to be selected.

#ifndef RANDOM_SELECTOR_HPP
#define RANDOM_SELECTOR_HPP

#include <string>
#include <vector>
#include <tuple>
#include <random>
#include "../base/path.hpp"
#include "../base/utils.hpp"
#include "../base/similarity_table.hpp"
using namespace std;

class Random_Selector{
	string START_LINE_COMPARATION_PRINT = "Functions: ";
	string BETWEEN_TWO_FUNCTION = " AND ";
	string BETWEEN_RELATIVE_AND_FUNCTION_NAME = "::";
	string SIMILARITY_MESSAGE = ". Similarity: ";
	
	Similarity_Table *similarity_table;
	double minimum_similarity;
	double maximum_similarity;
	double maximum_quantity;
	
	const int seed = 123456789;
	mt19937 rng = mt19937(seed);
	
	int processed_results = 0;

	Utils::COLOR choose_text_color();

	string format_path_message_in_pair(Path path);

	bool is_valid_pair(tuple<double,Path,Path> path_pair);

	vector<tuple<double,Path,Path>> get_similarity_pairs_filtered();

	vector<tuple<double,Path,Path>> make_random_selection(vector<tuple<double,Path,Path>> path_pairs);

	void print_path_pair(tuple<double,Path,Path> path_pair);

	void print_path_pairs(vector<tuple<double,Path,Path>> path_pairs);
	public:
	Random_Selector(
			Similarity_Table *_similarity_table,
			double _minimum_similarity,
			double _maximum_similarity,
			double _maximum_quantity
	);
};

#endif
