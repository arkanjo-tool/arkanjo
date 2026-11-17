/*
This is a Trie with a element being a folder to enable count of duplication code in the scope of files.
It is not simple to understand if you do not know the structure. Please refer to https://en.wikipedia.org/wiki/Trie .
*/
#include "counter_duplication_code_trie.hpp"
#include <arkanjo/formatter/format_manager.hpp>

Utils::Color CounterDuplicationCodeTrie::choose_text_color() {
    number_printed_lines++;
    Utils::Color ret = Utils::Color::BLACK;
    if (number_printed_lines % 2 == 0) {
        ret = Utils::Color::BLUE;
    }
    return ret;
}

int CounterDuplicationCodeTrie::create_node_at_the_end() {
    int id = trie.size();
    std::map<std::string, int> aux;
    trie.push_back(aux);
    counter_duplication_lines.push_back(kZeroInitialCounter);
    return id;
}

void CounterDuplicationCodeTrie::create_edge_if_not_exist(int node, const std::string& folder) {
    if (trie[node].find(folder) == trie[node].end()) {
        trie[node][folder] = create_node_at_the_end();
    }
}

std::string CounterDuplicationCodeTrie::create_context_string_on_depth(int depth) {
    std::string ret = "";
    for (int i = 0; i < depth; i++) {
        ret += kBasicShiftPerDepth;
    }
    ret += kEndArrow;
    return ret;
}

void CounterDuplicationCodeTrie::print_node_information(int node, int depth, const std::string& folder) {
    std::string line = create_context_string_on_depth(depth) + folder + kTwoPointerAfterFolder;
    line += std::to_string(counter_duplication_lines[node]) + kLineText;

    std::cout << FormatterManager::get_formatter()->colorize(line, choose_text_color()) << '\n';
}

void CounterDuplicationCodeTrie::dfs_print_duplication_code_trie(int current_node, int depth, const std::string& folder) {
    print_node_information(current_node, depth, folder);
    for (auto [child_folder, child_node] : trie[current_node]) {
        int child_depth = depth + 1;
        dfs_print_duplication_code_trie(child_node, child_depth, child_folder);
    }
}

void CounterDuplicationCodeTrie::add_folder_duplication_code(const std::vector<std::string>& folder_path, int number_of_duplication_lines) {
    int current_node = kRootNode;
    counter_duplication_lines[current_node] += number_of_duplication_lines;
    for (const auto& folder : folder_path) {
        create_edge_if_not_exist(current_node, folder);
        current_node = trie[current_node][folder];
        counter_duplication_lines[current_node] += number_of_duplication_lines;
    }
}

void CounterDuplicationCodeTrie::print_duplication_code_trie() {
    dfs_print_duplication_code_trie(kRootNode, kBaseDepth, kEmptyFolder);
}

CounterDuplicationCodeTrie::CounterDuplicationCodeTrie() {
    create_node_at_the_end();
}
