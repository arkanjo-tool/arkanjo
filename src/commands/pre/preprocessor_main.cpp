// As preprocessor is, in the moment, separated from orchestrator
// we define a main function to use it.

#include "preprocessor.hpp"
#include <cassert>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

int main(void) {
    Preprocessor preprocessor(true);
    return 0;
}
