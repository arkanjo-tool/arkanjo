#include "random_selector.hpp"

Utils::COLOR RandomSelector::choose_text_color() {
    Utils::COLOR ret = Utils::GRAY;
    if (processed_results % 2 == 0) {
        ret = Utils::CYAN;
    }
    return ret;
}

bool RandomSelector::is_valid_pair(tuple<double, Path, Path> path_pair) {
    auto [similarity, path1, path2] = path_pair;
    if (similarity < minimum_similarity)
        return false;
    if (similarity > maximum_similarity)
        return false;
    return true;
}

vector<tuple<double, Path, Path>> RandomSelector::get_similarity_pairs_filtered() {
    auto path_pairs = similarity_table->get_all_path_pairs_and_similarity_sorted_by_similarity();
    vector<tuple<double, Path, Path>> ret;
    for (auto path_pair : path_pairs) {
        if (is_valid_pair(path_pair)) {
            ret.push_back(path_pair);
        }
    }
    return ret;
}

vector<tuple<double, Path, Path>> RandomSelector::make_random_selection(vector<tuple<double, Path, Path>> path_pairs) {
    shuffle(path_pairs.begin(), path_pairs.end(), rng);
    while (int(path_pairs.size()) > maximum_quantity) {
        path_pairs.pop_back();
    }
    return path_pairs;
}

void RandomSelector::print_path_pair(tuple<double, Path, Path> path_pair) {
    auto [similarity, path1, path2] = path_pair;
    string line;
    line += START_LINE_COMPARATION_PRINT;
    line += path1.format_path_message_in_pair();
    line += BETWEEN_TWO_FUNCTION;
    line += path2.format_path_message_in_pair();
    line += SIMILARITY_MESSAGE;
    line += to_string(similarity);
    Utils::COLOR color = choose_text_color();
    cout << Utils::format_colored_message(line, color) << '\n';
}

void RandomSelector::print_path_pairs(vector<tuple<double, Path, Path>> path_pairs) {
    for (auto path_pair : path_pairs) {
        print_path_pair(path_pair);
    }
}

RandomSelector::RandomSelector(Similarity_Table* _similarity_table) {
    similarity_table = _similarity_table;
    similarity_table->update_similarity(0);
    minimum_similarity = 0;
    maximum_similarity = 0;
    maximum_quantity = 0;
}

bool RandomSelector::validate(const ParsedOptions& options) {
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
