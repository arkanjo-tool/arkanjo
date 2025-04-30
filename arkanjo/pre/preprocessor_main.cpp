// As preprocessor is, in the moment, separated from orchestrator
// we define a main function to use it.

#include "preprocessor.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <filesystem>
using namespace std;

int main(int argc, char *argv[]){
	Preprocessor preprocessor(true);
	return 0;
}
