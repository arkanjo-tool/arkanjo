#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <arkanjo/base/utils.hpp>
#include "../../commands/pre/preprocessor.hpp"
#include "../../base/config.hpp"

using namespace std;

const string EXPECTED_DIR = "tests/e2e/expected";
const string CURRENT_DIR = "tests/e2e/current";
const string CODEBASE_DIR = "tests/e2e/codebase";

vector<string> skip_check_list = {"/tmp/config.txt","/tmp/output_tool.txt","/tmp/output_parsed.txt"};

bool should_skip(string s){
	for(auto x : skip_check_list){
		if(x == s){
			return true;
		}
	}
	return false;
}

bool areEqualFile(string file1, string file2){
	vector<string> left = Utils::read_file_generic(file1);
	vector<string> right = Utils::read_file_generic(file2);
	
	if(left.size() != right.size()){	
		return false;
	}

	int sz = left.size();

	for(int i = 0; i < sz; i++){
		if(left[i] != right[i]){
			return false;
		}
	}

	return true;
}

string remove_prefix(string a, int rem){
	reverse(a.begin(),a.end());
	int cnt = rem;

	while(cnt > 0 && !a.empty()){
		a.pop_back();
		cnt--;
	}
	reverse(a.begin(),a.end());
	return a;
}

void Test(){
	vector<string> expected_files;
	vector<string> current_files;

	for(const auto &dirEntry: std::filesystem::recursive_directory_iterator(EXPECTED_DIR)){
		string file_path = dirEntry.path().string();
		expected_files.push_back(file_path);
	}
	for(const auto &dirEntry: std::filesystem::recursive_directory_iterator(CURRENT_DIR)){
		string file_path = dirEntry.path().string();
		current_files.push_back(file_path);
	}

	sort(expected_files.begin(),expected_files.end());
	sort(current_files.begin(),current_files.end());
	
	int ite = min((int)expected_files.size(),(int)current_files.size());
	
	for(int i = 0; i < ite; i++){
		string expected_file = expected_files[i];
		string current_file = current_files[i];

		string expected_without_prefix = remove_prefix(expected_file,EXPECTED_DIR.size());
		string current_without_prefix = remove_prefix(current_file,CURRENT_DIR.size());

		if(expected_without_prefix != current_without_prefix){
			if(expected_without_prefix < current_without_prefix){
				cout << "TEST FAILED" << '\n';
				cout << "Expected file with name " <<expected_file <<" but does not exist" << '\n';
				return;
			}else{
				cout << "TEST FAILED" << '\n';
				cout << "Unexpected file with name " << current_file << " found" << '\n';
				return;
			}
		}
		if(should_skip(current_without_prefix)) continue;
		if(!areEqualFile(expected_file,current_file)){
			cout << "TEST FAILED" << '\n';
			cout << "File " << expected_file << " does not have the expected content" << '\n';
			return;
		}
	}
	cout << "TEST PASSED" << '\n';
}


int main( int argc, char *argv[] ){
	Config::config()->setTestConfig();
	Preprocessor(true,CODEBASE_DIR,0);
	Test();

}

