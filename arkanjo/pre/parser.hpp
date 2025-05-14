// The parser is responsible to read the output to the code duplication detection tool
// that exists in the third_party folder and parse/transform it in a way that is easier
// to work with later.

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <set>
#include <fstream>
using namespace std;

struct Comparation{
	string path1, path2;
	double similarity;


	Comparation(){}

	Comparation(string _path1, string _path2, double _sim);
	bool operator<(const Comparation &com) const;
	bool operator==(const Comparation &com) const;

};


class Parser{
		
	private:
		string PROJECT_PATH_MESSAGE = "Enter your project path:";
		string MINIMUM_SIMILARITY_MESSAGE = "Enter minimum similarity desired on using the tool:";

		ifstream fin;
		ofstream fout;
		double similarity_cap_;

		vector<string> parser_line(string line);
		bool is_an_file(string s);
		string remove_formatation_from_similarity(string s);
		double retrive_similarity(string s);
		void parser_block(string path, set<Comparation> &comparations);
		void exec();

	public:
		Parser(string input_file, string output_file, double similarity_cap);
};

#endif
