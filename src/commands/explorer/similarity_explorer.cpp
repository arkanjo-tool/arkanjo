/*
This file expect the output of parser.cpp
This code filter the output only printing the files similar to files that contains a given pattern passed as argc
The code filter every file that has the pattern as a substring, so be carefull with duplications
*/

#include <algorithm>
#include <iostream>
#include <utility>

#include <arkanjo/formatter/format_manager.hpp>
#include "similarity_explorer.hpp"

using fm = FormatterManager;

int SimilarityExplorer::find_number_pairs_show(int number_pair_found) const {
    if (limit_on_results == UNLIMITED_RESULTS) {
        return number_pair_found;
    }
    return std::min(limit_on_results, number_pair_found);
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

SimilarityExplorerEntry SimilarityExplorer::process_similar_path_pair(const Path& path1, const Path& path2) {
    if (!match_pattern(path1, path2)) {
        return {};
    }
    if (limit_on_results != UNLIMITED_RESULTS && processed_results >= limit_on_results) {
        return {};
    }
    processed_results++;

    Function function1(path1);
    function1.load();

    Function function2(path2);
    function2.load();

    return {
        .path_a = path1.format_path_message_in_pair(),
        .path_b = path2.format_path_message_in_pair(),
        .dir_a = path1.build_relative_path().parent_path().string(),
        .dir_b = path2.build_relative_path().parent_path().string(),
        .filename_a = path1.build_relative_path().filename().string(),
        .filename_b = path2.build_relative_path().filename().string(),
        .func_a = path1.build_function_name(),
        .func_b = path2.build_function_name(),
        .start_a = function1.get_scope_function_in_file()[0],
        .start_b = function2.get_scope_function_in_file()[0],
        .end_a = function1.get_scope_function_in_file()[2],
        .end_b = function2.get_scope_function_in_file()[2],
        .duplicated_lines = function1.number_of_lines()
    };
}

int SimilarityExplorer::find_number_pair_found(const std::vector<SimilarPair>& similar_path_pairs) const {
    int count = 0;
    for (const auto& similar_pair : similar_path_pairs) {
        const Path& path1 = similarity_table->get_path(similar_pair.id1);
        const Path& path2 = similarity_table->get_path(similar_pair.id2);

        if (match_pattern(path1, path2)) {
            count++;
        }
    }
    return count;
}

std::vector<SimilarPair> SimilarityExplorer::build_similar_path_pairs() {
    std::vector<SimilarPair> similar_path_pairs = similarity_table->get_all_similar_pairs();
    if (sorted_by_number_of_duplicated_code) {
        similarity_table->sort_pairs_by_line_number(similar_path_pairs);
    } else {
        similarity_table->sort_pairs_by_similarity(similar_path_pairs);
    }
    return similar_path_pairs;
}

void SimilarityExplorer::explorer_clusters() {
    auto clusters_info = similarity_table->get_clusters_info(sorted_by_number_of_duplicated_code);

    int clusters_to_show = limit_on_results > 0 
        ? std::min(limit_on_results, (int)clusters_info.size()) 
        : (int)clusters_info.size();

    for (int idx = 0; idx < clusters_to_show; idx++) {
        const auto& info = clusters_info[idx];
        fm::write("Cluster #" + std::to_string(idx + 1)
          + " (Files: " + std::to_string(info.paths.size())
          + ", Pairs: " + std::to_string(info.total_pairs)
          + ", Lines: " + std::to_string(info.total_lines)
          + ", Score: " + std::to_string(info.score()) + ")");
        fm::write(Utils::LIMITER_PRINT);

        std::vector<SimilarityExplorerEntry> entries{};
        for (const auto& path : info.paths) {
            entries.push_back({
                path.format_path_message_in_pair(), "", 
                path.build_relative_path().parent_path().string(), "",
                path.build_relative_path().filename().string(), "",
                path.build_function_name(), "",
                find_number_lines(path)
            });
        }

        fm::write(TEMPLATE_PROCESSED_RESULTS_CLUSTERS, entries, Format::AUTO, [](size_t i) {
            return (i % 2 == 0)
                ? fm::get_formatter()->style().at("row_even")
                : fm::get_formatter()->style().at("row_odd");
        });
        fm::write("");
    }
}

void SimilarityExplorer::explorer() {
    auto start = std::chrono::high_resolution_clock::now();

    auto similar_path_pairs = build_similar_path_pairs();

    int number_pair_found = find_number_pair_found(similar_path_pairs);
    int number_pairs_show = find_number_pairs_show(number_pair_found);

    fm::write(TEMPLATE_INITIAL_TEXT, SimilarityExplorerInitialMessage{
        number_pair_found, number_pairs_show
    }, Format::TEXT);
    fm::write(Utils::LIMITER_PRINT);

    std::vector<SimilarityExplorerEntry> vector_entry = {};
    for (const auto& similar_pair : similar_path_pairs) {
        const Path& path1 = similarity_table->get_path(similar_pair.id1);
        const Path& path2 = similarity_table->get_path(similar_pair.id2);

        auto entry = process_similar_path_pair(path1, path2);
        if (entry.duplicated_lines < 0) continue;
        vector_entry.push_back(entry);
    }
    fm::write(template_processed_results_output, vector_entry, Format::AUTO, [](size_t i) {
        return (i % 2 == 0)
            ? fm::get_formatter()->style().at("row_even")
            : fm::get_formatter()->style().at("row_odd");
    });
}

SimilarityExplorer::SimilarityExplorer(Similarity_Table* table)
    : similarity_table(table),
      limit_on_results(0),
      pattern_to_match(""),
      both_path_need_to_match_pattern(false),
      sorted_by_number_of_duplicated_code(false),
      use_clusters(false) {}

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
    use_clusters = options.args.count("cluster") > 0;
    auto it_template = options.args.find("template");
    if (it_template != options.args.end()) {
        template_processed_results_output = it_template->second;
    }

    if (use_clusters) {
        explorer_clusters();
        return true;
    }
    explorer();
    return true;
}
