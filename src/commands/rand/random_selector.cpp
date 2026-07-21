#include "random_selector.hpp"
#include "random_selector_entry.hpp"
#include <arkanjo/base/function/function_loader.hpp>
#include <arkanjo/base/function/function_printer.hpp>
#include <arkanjo/formatter/format_manager.hpp>

using fm = FormatterManager;

bool RandomSelector::is_valid_pair(SimilarPair path_pair) {
    if (path_pair.similarity < minimum_similarity)
        return false;
    if (path_pair.similarity > maximum_similarity)
        return false;
    return true;
}

std::vector<SimilarPair> RandomSelector::get_similarity_pairs_filtered() {
    auto path_pairs = similarity_table->get_all_similar_pairs();
    similarity_table->sort_pairs_by_similarity(path_pairs);
    std::vector<SimilarPair> ret;
    for (auto path_pair : path_pairs) {
        if (is_valid_pair(path_pair)) {
            ret.push_back(path_pair);
        }
    }
    return ret;
}

std::vector<SimilarPair> RandomSelector::make_random_selection(std::vector<SimilarPair> path_pairs) {
    shuffle(path_pairs.begin(), path_pairs.end(), rng);
    while (int(path_pairs.size()) > maximum_quantity) {
        path_pairs.pop_back();
    }
    return path_pairs;
}

void RandomSelector::print_path_pairs(std::vector<SimilarPair> path_pairs) {
    std::vector<RandomSelectorEntry> vector_entry = {};
    FunctionLoader loader;
    for (const auto& path_pair : path_pairs) {
        const Path& path1 = similarity_table->get_path(path_pair.id1);
        const Path& path2 = similarity_table->get_path(path_pair.id2);

        auto function1 = loader.load_metadata(path1);
        auto function2 = loader.load_metadata(path2);

        vector_entry.push_back(RandomSelectorEntry{
            FunctionPrinter::format_path_message_in_pair(function1),
            FunctionPrinter::format_path_message_in_pair(function2),
            path_pair.similarity
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
