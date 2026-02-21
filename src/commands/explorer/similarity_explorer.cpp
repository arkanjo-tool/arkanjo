/*
This file expect the output of parser.cpp
This code filter the output only printing the files similar to files that contains a given pattern passed as argc
The code filter every file that has the pattern as a substring, so be carefull with duplications
*/

#include <algorithm>
#include <iostream>
#include <utility>

#include "similarity_explorer.hpp"

Utils::COLOR SimilarityExplorer::alternating_row_color(size_t index) const {
    return (index % 2 == 0) ? Utils::GRAY : Utils::CYAN;
}

int SimilarityExplorer::find_number_pairs_show(int number_pair_found) const {
    if (limit_on_results == UNLIMITED_RESULTS) {
        return number_pair_found;
    }
    return std::min(limit_on_results, number_pair_found);
}

std::string SimilarityExplorer::initial_message(int found, int show) const {
    return INITIAL_TEXT_PRINT_1 + std::to_string(found) +
           INITIAL_TEXT_PRINT_2 + std::to_string(show) +
           INITIAL_TEXT_PRINT_3;
}

bool SimilarityExplorer::match_pattern(const Path& path1, const Path& path2) const {
    bool match1 = path1.contains_given_pattern(pattern_to_match);
    bool match2 = path2.contains_given_pattern(pattern_to_match);

    if (both_path_need_to_match_pattern) {
        return match1 && match2;
    }
    return match1 || match2;
}

int SimilarityExplorer::find_number_lines(const Path& path1) {
    Function function(path1);
    function.load();
    return function.number_of_lines();
}

void SimilarityExplorer::print_similar_path_pair(const Path& path1, const Path& path2) {
    std::string row = START_LINE_COMPARATION_PRINT +
                      path1.format_path_message_in_pair() +
                      BETWEEN_TWO_FUNCTION +
                      path2.format_path_message_in_pair() +
                      NUMBER_LINES_MESSAGE +
                      std::to_string(find_number_lines(path1));

    std::cout << FormatterManager::get_formatter()->format(row, alternating_row_color(processed_results)) << '\n';
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
    for (const auto& [path1, path2] : similar_path_pairs) {
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
    int number_pair_found = find_number_pair_found(similar_path_pairs);
    int number_pairs_show = find_number_pairs_show(number_pair_found);

    std::cout << initial_message(number_pair_found, number_pairs_show) << '\n';
    std::cout << Utils::LIMITER_PRINT << '\n';

    for (const auto& [path1, path2] : similar_path_pairs) {
        process_similar_path_pair(path1, path2);
    }
}

SimilarityExplorer::SimilarityExplorer(Similarity_Table* table)
    : similarity_table(table),
      limit_on_results(0),
      pattern_to_match(""),
      both_path_need_to_match_pattern(false),
      sorted_by_number_of_duplicated_code(false) {}

bool SimilarityExplorer::validate(const ParsedOptions& options) {
    auto it_limiter = options.args.find("limiter");

    if (it_limiter != options.args.end()) {
        try {
            limit_on_results = std::stoi(it_limiter->second);
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
    auto it_pattern = options.args.find("pattern");
    if (it_pattern != options.args.end()) {
        pattern_to_match = it_pattern->second;
    }
    both_path_need_to_match_pattern = options.args.count("both-match") > 0;
    sorted_by_number_of_duplicated_code = options.args.count("sort") > 0;

    explorer();

    return true;
}