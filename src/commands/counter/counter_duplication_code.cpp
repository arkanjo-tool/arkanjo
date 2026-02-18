#include "counter_duplication_code.hpp"

void CounterDuplicationCode::mark_path_as_processed(Path path) {
    processed_paths.insert(path);
}

bool CounterDuplicationCode::is_path_processed_path(Path path) {
    return processed_paths.find(path) != processed_paths.end();
}

int CounterDuplicationCode::get_number_of_lines_in_path(const Path& path) {
    Function function(path);
    function.load();
    return function.number_of_lines();
}

void CounterDuplicationCode::register_code_duplication(const Path& path1, const Path& path2, int number_of_lines) {
    std::vector<std::string> common_folder_path = path1.get_common_folders(path2);
    counter_duplication_code_trie.add_folder_duplication_code(common_folder_path, number_of_lines);
}

void CounterDuplicationCode::process_path(const Path& path) {
    if (is_path_processed_path(path)) {
        return;
    }
    int number_of_lines = get_number_of_lines_in_path(path);
    mark_path_as_processed(path);
    std::vector<Path> similar_paths = similarity_table->get_similar_path_to_the_reference(path);
    for (const auto& similar_path : similar_paths) {
        mark_path_as_processed(similar_path);
        register_code_duplication(path, similar_path, number_of_lines);
    }
}

void CounterDuplicationCode::process_every_path_in_similarity_table() {
    std::vector<Path> paths = similarity_table->get_path_list();
    for (const auto& path : paths) {
        process_path(path);
    }
}

CounterDuplicationCode::CounterDuplicationCode(Similarity_Table* _similarity_table) {
    similarity_table = _similarity_table;
}

bool CounterDuplicationCode::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool CounterDuplicationCode::run([[maybe_unused]] const ParsedOptions& options) {
    process_every_path_in_similarity_table();
    counter_duplication_code_trie.print_duplication_code_trie();

    return true;
}
