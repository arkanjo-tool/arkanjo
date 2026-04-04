#include <iostream>

#include <arkanjo/formatter/format_manager.hpp>
#include "similar_function_finder.hpp"
#include "similar_function_finder_entry.hpp"

using fm = FormatterManager;

void SimilarFunctionFinder::find_path_that_meets_pattern() {
    std::vector<Path> paths = similarity_table->get_path_list();
    for (const auto& _path : paths) {
        if (_path.contains_given_pattern(function_pattern)) {
            path = _path;
        }
    }
}

void SimilarFunctionFinder::print_empty_path_message() const {
    fm::write(TEMPLATE_EMPTY_FUNCTION, SimilarFunctionEmptyEntry{
        function_pattern
    }, Format::TEXT);
}

void SimilarFunctionFinder::print_function(const Path& path) const {
    Function function(path);
    function.load();
    function.print_basic_info();
}

void SimilarFunctionFinder::print_reference_path() {
    fm::write(REFERENCE_PATH_MESSAGE);
    print_function(path);
}

void SimilarFunctionFinder::print_similar_functions(const std::vector<Path>& similar_paths) {
    fm::write(TEMPLATE_COUNT, SimilarFunctionCountEntry{
        similar_paths.size()
    }, Format::TEXT);
    for (const auto& similar_path : similar_paths) {
        print_function(similar_path);
    }
}

void SimilarFunctionFinder::print_similar_functions() {
    if (path.is_empty()) {
        print_empty_path_message();
        return;
    }
    std::vector<Path> similar_paths = similarity_table->get_similar_path_to_the_reference(path);

    fm::write(Utils::LIMITER_PRINT);
    print_reference_path();
    print_similar_functions(similar_paths);
}

SimilarFunctionFinder::SimilarFunctionFinder(Similarity_Table* _similarity_table) {
    similarity_table = _similarity_table;
    function_pattern = "";
}

bool SimilarFunctionFinder::validate(const ParsedOptions& options) {
    if (options.extra_args.empty()) {
        throw CLIError("Similar Function Finder Command expect one parameter, but none was given");
        return false;
    }

    return true;
}

bool SimilarFunctionFinder::run(const ParsedOptions& options) {
    function_pattern = options.extra_args[0];

    find_path_that_meets_pattern();
    print_similar_functions();

    return true;
}
