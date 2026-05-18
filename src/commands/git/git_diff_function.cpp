#include "git_diff_function.hpp"

#include <git2.h>

#include <iostream>
#include <sstream>

#include <arkanjo/formatter/format_manager.hpp>
#include <arkanjo/base/function/function.hpp>
#include <arkanjo/cli/cli_error.hpp>

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

    std::cout << "Comparing functions: '" << first_path.build_function_name() << "' and '"
              << second_path.build_function_name() << "'\n";

    if (!similarity_table->is_similar(first_path, second_path)) {
        std::cerr << "The selected functions are not flagged as duplicates in the similarity table.\n";
        return false;
    }

    return diff_functions(first_path, second_path);
}

std::vector<Path> GitDiffFunction::find_matching_paths(const std::string& pattern) const {
    std::vector<Path> matches;
    for (const auto& path : similarity_table->get_path_list()) {
        if (path.contains_given_pattern(pattern)) {
            matches.push_back(path);
        }
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
        for (const auto& path : candidates) {
            message << "  - " << path.format_path_message_in_pair() << "\n";
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
        case GIT_DIFF_LINE_FILE_HDR:
        case GIT_DIFF_LINE_HUNK_HDR:
            output_line = formatter->colorize(content, Utils::COLOR::CYAN);
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
    Function first_function(first);
    Function second_function(second);
    first_function.load();
    second_function.load();

    std::string first_text = build_text_from_lines(first_function.build_all_content());
    std::string second_text = build_text_from_lines(second_function.build_all_content());

    git_libgit2_init();

    git_diff_options diff_options = GIT_DIFF_OPTIONS_INIT;

    std::string old_as_path = "a/" + first.build_function_name();
    std::string new_as_path = "b/" + second.build_function_name();
    diff_options.old_prefix = old_as_path.c_str();
    diff_options.new_prefix = new_as_path.c_str();

    DiffOutputPayload payload{&std::cout, false};
    int error = git_diff_buffers(
        first_text.c_str(), first_text.size(), first.build_function_name().c_str(),
        second_text.c_str(), second_text.size(), second.build_function_name().c_str(),
        &diff_options,
        nullptr,
        nullptr,
        nullptr,
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
