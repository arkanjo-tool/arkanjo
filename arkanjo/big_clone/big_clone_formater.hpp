// This files is related to the first try to evaluate the tool in big clone bench dataset
// the class does not work properly in the moment and should be ignored.

#ifndef BIG_CLONE_FORMATER_HPP
#define BIG_CLONE_FORMATER_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <tuple>
#include <cassert>

#include "../base/path.hpp"
#include "../base/function.hpp"
#include "../base/similarity_table.hpp"

using namespace std;

// This class expects to be executed in the context of the folders of the BigCloneBench dataset
class Big_Clone_Formater {
    string format_relative_path(string relative_path);
    string build_path_formated_string(Path path);
    auto process_similar_path_pair(Path path1, Path path2, double similarity);

public:
    Big_Clone_Formater(Similarity_Table *similarity_table);
};

#endif
