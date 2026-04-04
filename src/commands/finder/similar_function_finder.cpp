#include <iostream>

#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/commands/pre/preprocessor.hpp>
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

Function SimilarFunctionFinder::get_reference_function(const Path& path) const {
    Function function(path);
    function.load();
    return function;
}

void SimilarFunctionFinder::print_reference_path() {
    fm::write(REFERENCE_PATH_MESSAGE);
    auto function = get_reference_function(path);
    function.print_basic_info();
}

void SimilarFunctionFinder::print_similar_functions(const std::vector<Path>& similar_paths) {
    fm::write(TEMPLATE_COUNT, SimilarFunctionCountEntry{
        similar_paths.size()
    }, Format::TEXT);
    fm::write("");
    for (const auto& similar_path : similar_paths) {
        auto function = get_reference_function(similar_path);
        fm::write(Utils::LIMITER_PRINT);
        function.print_basic_info();
        fm::write(Utils::LIMITER_PRINT);
        fm::write("");
    }
}

void SimilarFunctionFinder::print_similar_functions() {
    std::vector<Path> similar_paths = similarity_table->get_similar_path_to_the_reference(path);

    fm::write(Utils::LIMITER_PRINT);
    print_reference_path();
    fm::write(Utils::LIMITER_PRINT);
    if (open_folder) {
        auto params = Preprocessor::read_current_run_params();
        fs::path full_path = fs::path{params[0]} / fs::path{path.build_relative_path()};
        fs::path dir_path = full_path.parent_path();
        Utils::open_folder(dir_path);
        return;
    }
    print_similar_functions(similar_paths);
}

void SimilarFunctionFinder::handle_show_mode() {
    Function ref = get_reference_function(path);
    ref.print_basic_info();
    fm::write("");
    ref.print_code(no_numbers);
}

SimilarFunctionFinder::SimilarFunctionFinder(Similarity_Table* _similarity_table) {
    similarity_table = _similarity_table;
    function_pattern = "";
}

bool SimilarFunctionFinder::validate(const ParsedOptions& options) {
    open_folder = options.args.count("open") > 0;
    show_mode = options.args.count("show") > 0;
    no_numbers = options.args.count("no-numbers") > 0;

    if (options.args.count("help") > 0) {
        return true;
    }

    if (options.extra_args.empty()) {
        throw CLIError("Similar Function Finder Command expect one parameter, but none was given");
        return false;
    }

    return true;
}

bool SimilarFunctionFinder::run(const ParsedOptions& options) {
    function_pattern = options.extra_args[0];

    find_path_that_meets_pattern();

    if (path.is_empty()) {
        print_empty_path_message();
        return true;
    }

    if (show_mode) {
        handle_show_mode();
        return true;
    }

    print_similar_functions();

    return true;
}
