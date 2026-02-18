/*
This file expect the output of parser.cpp
This code filter the output only printing the files similar to files that contains a given pattern passed as argc
The code filter every file that has the pattern as a substring, so be carefull with duplications
*/

#include <algorithm>
#include <iostream>
#include <utility>

#include "similarity_explorer.hpp"

Utils::COLOR SimilarityExplorer::choose_text_color() const {
    Utils::COLOR ret = Utils::GRAY;
    if (processed_results % 2 == 0) {
        ret = Utils::CYAN;
    }
    return ret;
}

int SimilarityExplorer::find_number_pairs_show(int number_pair_found) const {
    if (limit_on_results == UNLIMITED_RESULTS) {
        return number_pair_found;
    }
    return std::min(limit_on_results, number_pair_found);
}

std::string SimilarityExplorer::format_initial_message(int number_pair_found) const {
    std::string ret;
    ret += INITIAL_TEXT_PRINT_1;
    ret += std::to_string(number_pair_found);
    ret += INITIAL_TEXT_PRINT_2;
    ret += std::to_string(find_number_pairs_show(number_pair_found));
    ret += INITIAL_TEXT_PRINT_3;
    return ret;
}

bool SimilarityExplorer::match_pattern(const Path& path1, const Path& path2) const {
    bool match1 = path1.contains_given_pattern(pattern_to_match);
    bool match2 = path2.contains_given_pattern(pattern_to_match);

    if (both_path_need_to_match_pattern) {
        return match1 && match2;
    }
    return match1 || match2;
}

std::string SimilarityExplorer::format_path_message_in_pair(const Path& path) const {
    std::string ret = path.build_relative_path() + BETWEEN_RELATIVE_AND_FUNCTION_NAME + path.build_function_name();
    return ret;
}

int SimilarityExplorer::find_number_lines(const Path& path1) {
    Function function(path1);
    function.load();
    return function.number_of_lines();
}

void SimilarityExplorer::print_similar_path_pair(const Path& path1, const Path& path2) {
    std::string line;
    line += START_LINE_COMPARATION_PRINT;
    line += format_path_message_in_pair(path1);
    line += BETWEEN_TWO_FUNCTION;
    line += format_path_message_in_pair(path2);
    line += NUMBER_LINES_MESSAGE;
    line += std::to_string(find_number_lines(path1));

    Utils::COLOR color = choose_text_color();
    std::cout << Utils::format_colored_message(line, color) << '\n';
}

void SimilarityExplorer::process_similar_path_pair(const Path& path1, const Path& path2) {
    if (!match_pattern(path1, path2)) {
        return;
    }
    if (limit_on_results != UNLIMITED_RESULTS && processed_results >= limit_on_results) {
        return;
    }
    processed_results++;
    print_similar_path_pair(path1, path2);
}

int SimilarityExplorer::find_number_pair_found(const std::vector<std::pair<Path, Path>>& similar_path_pairs) const {
    int count = 0;
    for (auto [path1, path2] : similar_path_pairs) {
        if (match_pattern(path1, path2)) {
            count++;
        }
    }
    return count;
}

std::vector<std::pair<Path, Path>> SimilarityExplorer::build_similar_path_pairs() {
    std::vector<std::pair<Path, Path>> similar_path_pairs;
    if (sorted_by_number_of_duplicated_code) {
        similar_path_pairs = similarity_table->get_all_similar_path_pairs_sorted_by_line_number();
    } else {
        similar_path_pairs = similarity_table->get_all_similar_path_pairs_sorted_by_similarity();
    }
    return similar_path_pairs;
}

void SimilarityExplorer::explorer() {
    std::vector<std::pair<Path, Path>> similar_path_pairs = build_similar_path_pairs();
    std::string initial_line = format_initial_message(find_number_pair_found(similar_path_pairs));

    std::cout << initial_line << '\n';
    std::cout << Utils::LIMITER_PRINT << '\n';

    for (auto [path1, path2] : similar_path_pairs) {
        process_similar_path_pair(path1, path2);
    }
}

SimilarityExplorer::SimilarityExplorer(Similarity_Table* _similarity_table) 
    : ICommand(
        std::vector<struct option>{
            {"limiter", required_argument, nullptr, 'l'},
            {"pattern", required_argument, nullptr, 'p'},
            {"both-match", no_argument, nullptr, 'b'},
            {"sort", no_argument, nullptr, 'c'}
        },
        "l:p:bc" // short options
    ) {
    similarity_table = _similarity_table;
    limit_on_results = 0;
    pattern_to_match = "";
    both_path_need_to_match_pattern = false;
    sorted_by_number_of_duplicated_code = false;
}

bool SimilarityExplorer::validate(const ParsedOptions& options) {
    auto it_limiter = options.args.find("limiter");

    if (it_limiter != options.args.end()) {
        try {
            std::stoi(it_limiter->second);
        } catch (const std::invalid_argument&) {
            throw CLIError("--limiter must be a valid number (passing " + it_limiter->second + ")");
            return false;
        } catch (const std::out_of_range&) {
            throw CLIError("--limiter outside the permitted range");
            return false;
        }
    }

    return true;
}

bool SimilarityExplorer::run(const ParsedOptions& options) {
    auto it_limiter = options.args.find("limiter");
    if (it_limiter != options.args.end()) {
        limit_on_results = std::stoi(it_limiter->second);
    }
    auto it_pattern = options.args.find("pattern");
    if (it_pattern != options.args.end()) {
        pattern_to_match = it_pattern->second;
    }
    both_path_need_to_match_pattern = options.args.count("both-match") > 0;
    sorted_by_number_of_duplicated_code = options.args.count("sort") > 0;

    explorer();

    return true;
}