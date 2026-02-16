#include <iostream>

#include "similar_function_finder.hpp"

void Similar_Function_Finder::find_path_that_meets_pattern() {
    std::vector<Path> paths = similarity_table->get_path_list();
    for (const auto& _path : paths) {
        if (_path.contains_given_pattern(function_pattern)) {
            path = _path;
        }
    }
}

void Similar_Function_Finder::print_empty_path_message() const {
    std::string line1 = EMPTY_PATH_MESSAGE_1 + function_pattern;
    std::string line2 = EMPTY_PATH_MESSAGE_2;
    std::cout << line1 << '\n';
    std::cout << line2 << '\n';
}

void Similar_Function_Finder::print_function(const Path& path) const {
    Function function(path);
    function.load();
    function.print_basic_info();
}

void Similar_Function_Finder::print_reference_path() {
    std::string line1 = REFERENCE_PATH_MESSAGE;
    std::cout << line1 << '\n';
    print_function(path);
}

void Similar_Function_Finder::print_similar_functions(const std::vector<Path>& similar_paths) {
    std::string line1 = COUNT_MESSAGE_1 + std::to_string(similar_paths.size()) + COUNT_MESSAGE_2;
    std::cout << line1 << '\n';
    for (const auto& similar_path : similar_paths) {
        print_function(similar_path);
    }
}

void Similar_Function_Finder::print_similar_functions() {
    if (path.is_empty()) {
        print_empty_path_message();
        return;
    }
    std::vector<Path> similar_paths = similarity_table->get_similar_path_to_the_reference(path);

    std::cout << Utils::LIMITER_PRINT << '\n';
    print_reference_path();
    print_similar_functions(similar_paths);
}

Similar_Function_Finder::Similar_Function_Finder(const std::string& _function_pattern, Similarity_Table* _similarity_table) : function_pattern(_function_pattern) {
    similarity_table = _similarity_table;
}

void Similar_Function_Finder::run() {
    find_path_that_meets_pattern();
    print_similar_functions();
}
