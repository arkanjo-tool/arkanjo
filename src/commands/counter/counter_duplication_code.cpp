#include <arkanjo/base/function/function_loader.hpp>

#include "counter_duplication_code.hpp"

std::vector<std::string> CounterDuplicationCode::common_folders(
    const Path& path1, const Path& path2
) const {
    std::vector<std::string> tokens_relative_1;
    std::vector<std::string> tokens_relative_2;

    for (const auto& part : path1.value())
        tokens_relative_1.push_back(part.string());
    for (const auto& part : path2.value())
        tokens_relative_2.push_back(part.string());

    size_t minimum_size_tokens = std::min(tokens_relative_1.size(), tokens_relative_2.size());
    std::vector<std::string> common_folders;
    for (size_t i = 0; i < minimum_size_tokens; i++) {
        auto token_1 = tokens_relative_1[i];
        auto token_2 = tokens_relative_2[i];
        if (token_1 == token_2)
            common_folders.push_back(token_1);
        else
            break;
    }
    return common_folders;
}

void CounterDuplicationCode::mark_path_as_processed(Path path) {
    processed_paths.insert(path);
}

bool CounterDuplicationCode::is_path_processed_path(Path path) {
    return processed_paths.find(path) != processed_paths.end();
}

int CounterDuplicationCode::get_number_of_lines_in_path(const Path& path) {
    FunctionLoader loader;
    auto function = loader.load_metadata(path);
    return function.scope_location().size();
}

void CounterDuplicationCode::register_code_duplication(const Path& path1, const Path& path2, int number_of_lines) {
    std::vector<std::string> common_folder_path = common_folders(path1, path2);
    counter_duplication_code_trie.add_folder_duplication_code(common_folder_path, number_of_lines);
}

void CounterDuplicationCode::process_path(const Path& path) {
    if (is_path_processed_path(path)) {
        return;
    }
    uint32_t number_of_lines = get_number_of_lines_in_path(path);
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

bool CounterDuplicationCode::validate(const ParsedOptions& options) {
    if (options.has("json"))
        throw CLIError("--json is not supported in this command.");

    return true;
}

bool CounterDuplicationCode::run([[maybe_unused]] const ParsedOptions& options) {
    process_every_path_in_similarity_table();
    counter_duplication_code_trie.print_duplication_code_trie();

    return true;
}
