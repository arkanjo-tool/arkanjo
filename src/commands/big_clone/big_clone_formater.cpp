#include "big_clone_formater.hpp"

#include <iomanip>
#include <iostream>

#include <arkanjo/base/function.hpp>

std::string BigCloneFormater::format_relative_path(std::string relative_path) {
    for (auto& c : relative_path) {
        if (c == '/') {
            c = ',';
        }
    }
    return relative_path;
}

std::string BigCloneFormater::build_path_formated_string(Path path) {
    std::string relative_path = path.build_relative_path();
    relative_path = format_relative_path(relative_path);

    Function function(path);
    function.load();
    auto [line_declaration, start_line, end_line] = function.get_scope_function_in_file();

    std::string ret;
    ret += relative_path;
    ret += ',' + std::to_string(line_declaration);
    ret += ',' + std::to_string(start_line);
    ret += ',' + std::to_string(end_line);

    return ret;
}
auto BigCloneFormater::process_similar_path_pair(Path path1, Path path2, double similarity) {
    auto string_path1 = build_path_formated_string(path1);
    auto string_path2 = build_path_formated_string(path2);

    std::string comparation_string = string_path1 + ',' + string_path2 + ',';
    std::cout << comparation_string;
    std::cout << std::fixed << std::setprecision(2) << similarity << '\n';
}

BigCloneFormater::BigCloneFormater(Similarity_Table* _similarity_table) {
    similarity_table = _similarity_table;
}

bool BigCloneFormater::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool BigCloneFormater::run([[maybe_unused]] const ParsedOptions& options) {
    auto similar_paths = similarity_table->get_all_path_pairs_and_similarity_sorted_by_similarity();
    for (auto [similarity, path1, path2] : similar_paths) {
        process_similar_path_pair(path1, path2, similarity);
    }

    return true;
}