#include <arkanjo/formatter/format_manager.hpp>
#include "random_selector.hpp"
#include "random_selector_entry.hpp"

#include <arkanjo/utils/utils.hpp>

using fm = FormatterManager;

bool RandomSelector::is_valid_pair(std::tuple<double, Path, Path> path_pair) {
    auto [similarity, path1, path2] = path_pair;
    if (similarity < minimum_similarity)
        return false;
    if (similarity > maximum_similarity)
        return false;
    return true;
}

std::vector<std::tuple<double, Path, Path>> RandomSelector::get_similarity_pairs_filtered() {
    auto path_pairs = similarity_table->get_all_path_pairs_and_similarity_sorted_by_similarity();
    std::vector<std::tuple<double, Path, Path>> ret;
    for (auto path_pair : path_pairs) {
        if (is_valid_pair(path_pair)) {
            ret.push_back(path_pair);
        }
    }
    return ret;
}

std::vector<std::tuple<double, Path, Path>> RandomSelector::make_random_selection(std::vector<std::tuple<double, Path, Path>> path_pairs) {
    shuffle(path_pairs.begin(), path_pairs.end(), rng);
    while (int(path_pairs.size()) > maximum_quantity) {
        path_pairs.pop_back();
    }
    return path_pairs;
}

void RandomSelector::print_path_pairs(std::vector<std::tuple<double, Path, Path>> path_pairs) {
    std::vector<RandomSelectorEntry> vector_entry = {};
    for (const auto& [similarity, path1, path2] : path_pairs) {
        vector_entry.push_back(RandomSelectorEntry{
            path1.format_path_message_in_pair(),
            path2.format_path_message_in_pair(),
            similarity
        });
    }
    if (vector_entry.size() <= 0) return;
    
    fm::write(TEMPLATE_RANDOM_ENTRY, vector_entry, Format::AUTO, [](size_t i) {
        return (i % 2 == 0)
            ? fm::get_formatter()->style().at("row_even")
            : fm::get_formatter()->style().at("row_odd");
    });
}

RandomSelector::RandomSelector(Similarity_Table* _similarity_table) {
    similarity_table = _similarity_table;
    similarity_table->update_similarity(0);
    minimum_similarity = 0;
    maximum_similarity = 0;
    maximum_quantity = 0;
}

bool RandomSelector::validate(const ParsedOptions& options) {
    if (options.args.count("help") > 0)
        return true;

    if (options.extra_args.size() <= 2) {
        throw CLIError("Random expect three parameters, but less was given");
        return false;
    }

    return true;
}

bool RandomSelector::run(const ParsedOptions& options) {
    minimum_similarity = stod(options.extra_args[0]);
    maximum_similarity = stod(options.extra_args[1]);
    maximum_quantity = stod(options.extra_args[2]);

    auto path_pairs = get_similarity_pairs_filtered();
    path_pairs = make_random_selection(path_pairs);
    print_path_pairs(path_pairs);

    return true;
}
