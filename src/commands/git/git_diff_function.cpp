#include "git_diff_function.hpp"

#include <git2.h>

#include <iostream>
#include <sstream>

#include <arkanjo/base/function/function_loader.hpp>
#include <arkanjo/base/function/function_printer.hpp>
#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/formatter/format_manager.hpp>

struct DiffOutputPayload {
    std::ostream* output;
    bool printed;
};

GitDiffFunction::GitDiffFunction(Similarity_Table* _similarity_table)
    : similarity_table(_similarity_table) {}

bool GitDiffFunction::validate(const ParsedOptions& options) {

    if (options.args.count("help") > 0) {
        return true;
    }

    if (options.extra_args.size() != 2) {
        throw CLIError("Git diff command expects two function name patterns.");
        return false;
    }
    return true;
}

bool GitDiffFunction::run(const ParsedOptions& options) {
    // Make a copy of the patterns to format them for searching in the similarity table
    const std::string first_pattern = format_pattern(options.extra_args[0]);
    const std::string second_pattern = format_pattern(options.extra_args[1]);

    std::vector<Path> first_candidates = find_matching_paths(first_pattern);
    std::vector<Path> second_candidates = find_matching_paths(second_pattern);

    Path first_path = choose_single_path(first_candidates, options.extra_args[0]);
    Path second_path = choose_single_path(second_candidates, options.extra_args[1]);

    if (!similarity_table->is_similar(first_path, second_path)) {
        auto similarity =similarity_table->get_threshold_similarity();

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << similarity;

        throw CLIError("The selected functions are not flagged as duplicates in the similarity table (similarity: " + ss.str() + ").");
        return false;
    }

    return diff_functions(first_path, second_path);
}

std::vector<Path> GitDiffFunction::find_matching_paths(const std::string& pattern) const {
    std::vector<Path> matches;
    FunctionLoader loader;
    for (const auto& path : similarity_table->get_path_list()) {
        auto function = loader.load_metadata(path);

        if (function.contains_given_pattern(pattern))
            matches.push_back(path);
    }
    return matches;
}

const std::string GitDiffFunction::format_pattern(const std::string& pattern) const {
    std::string result = pattern;

    size_t pos = 0;
    while ((pos = result.find("::", pos)) != std::string::npos) {
        result.replace(pos, 2, "/");
        pos += 1;
    }

    return result;
}

Path GitDiffFunction::choose_single_path(const std::vector<Path>& candidates, const std::string& pattern) const {
    if (candidates.empty()) {
        throw CLIError("No function found matching pattern: " + pattern);
    }
    if (candidates.size() > 1) {
        std::ostringstream message;
        message << "Multiple functions match the pattern '" << pattern << "':\n";
        FunctionLoader loader;
        for (const auto& path : candidates) {
            auto function = loader.load_metadata(path);
            message << "  - " << FunctionPrinter::format_path_message_in_pair(function) << "\n";
        }
        message << "Please use a more specific function name pattern.";
        throw CLIError(message.str());
    }
    return candidates.front();
}

std::string GitDiffFunction::build_text_from_lines(const std::vector<std::string>& lines) {
    std::string output;
    output.reserve(lines.size() * 80);
    for (const auto& line : lines) {
        output += line;
        output.push_back('\n');
    }
    return output;
}

int GitDiffFunction::print_diff_file(const git_diff_delta* delta, float, void* payload) {
    auto* data = static_cast<DiffOutputPayload*>(payload);

    if (!data->output)
        return 0;

    std::string old_file = "--- a/";
    old_file += delta->old_file.path;
    old_file += "\n";

    std::string new_file = "+++ b/";
    new_file += delta->new_file.path;
    new_file += "\n";

    data->output->write(old_file.data(), old_file.size());
    data->output->write(new_file.data(), new_file.size());

    return 0;
}

int GitDiffFunction::print_diff_hunk(const git_diff_delta*,const git_diff_hunk* hunk,void* payload) {
    auto* data = static_cast<DiffOutputPayload*>(payload);

    if (!data->output)
        return 0;

    auto formatter = FormatterManager::get_formatter();

    std::string header(hunk->header, hunk->header_len);
    header = formatter->colorize(header, Utils::COLOR::MAGENTA);

    data->output->write(header.data(), header.size());

    return 0;
}

int GitDiffFunction::print_diff_line(const git_diff_delta* /*delta*/, const git_diff_hunk* /*hunk*/, const git_diff_line* line, void* payload) {
    auto* data = static_cast<DiffOutputPayload*>(payload);
    data->printed = true;

    if (!data->output || line->content_len <= 0) {
        return 0;
    }

    auto formatter = FormatterManager::get_formatter();
    std::string output_line;
    std::string content(line->content, line->content_len);

    switch (line->origin) {
        case GIT_DIFF_LINE_ADDITION:
            output_line = formatter->colorize("+"+ content , Utils::COLOR::GREEN) ;
            break;
        case GIT_DIFF_LINE_DELETION:
            output_line = formatter->colorize("-" + content, Utils::COLOR::RED);
            break;
        case GIT_DIFF_LINE_CONTEXT:
            output_line = std::string(" ") + content;
            break;
        default:
            output_line = content;
            break;
    }

    /* 
        TODO: Perhaps should be a good idea use << operator instead of write, 
        but for that we need to handle the string termination character, 
        since content is not guaranteed to be null-terminated. 
    */
    data->output->write(output_line.data(), output_line.size());
    return 0;
}

bool GitDiffFunction::diff_functions(const Path& first, const Path& second) const {
    FunctionLoader loader;
    auto first_function = loader.load(first);
    auto second_function = loader.load(second);

    std::string first_text = build_text_from_lines(first_function.build_all_content());
    std::string second_text = build_text_from_lines(second_function.build_all_content());

    git_libgit2_init();

    git_diff_options diff_options = GIT_DIFF_OPTIONS_INIT;

    DiffOutputPayload payload{&std::cout, false};
    int error = git_diff_buffers(
        first_text.c_str(),
        first_text.size(),
        first_function.name().c_str(),
        second_text.c_str(),
        second_text.size(),
        second_function.name().c_str(),
        &diff_options,
        &GitDiffFunction::print_diff_file,
        nullptr,
        &GitDiffFunction::print_diff_hunk,
        &GitDiffFunction::print_diff_line,
        &payload
    );
    git_libgit2_shutdown();

    if (error != 0) {
        std::cerr << "Failed to compute diff between functions. libgit2 error code: " << error << "\n";
        return false;
    }

    if (!payload.printed) {
        std::cout << "No differences found between the selected functions.\n";
    }

    return true;
}
