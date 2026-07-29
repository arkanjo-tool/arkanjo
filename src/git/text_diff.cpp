#include <arkanjo/git/text_diff.hpp>

#include <arkanjo/formatter/format_manager.hpp>

#include <string>

struct DiffOutputPayload {
    std::ostream* output;
    bool printed = false;
};

int GitTextDiff::print_diff_file(const git_diff_delta* delta, float, void* payload) {
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

int GitTextDiff::print_diff_hunk(const git_diff_delta*,const git_diff_hunk* hunk,void* payload) {
    auto* data = static_cast<DiffOutputPayload*>(payload);

    if (!data->output)
        return 0;

    auto formatter = FormatterManager::get_formatter();

    std::string header(hunk->header, hunk->header_len);
    header = formatter->colorize(header, Utils::COLOR::MAGENTA);

    data->output->write(header.data(), header.size());

    return 0;
}

int GitTextDiff::print_diff_line(
    const git_diff_delta* /*delta*/,
    const git_diff_hunk* /*hunk*/,
    const git_diff_line* line,
    void* payload
) {
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
            output_line = formatter->colorize("+"+ content , Utils::COLOR::GREEN);
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

DiffResult GitTextDiff::compare(
    std::string_view first_text,
    std::string_view first_name,
    std::string_view second_text,
    std::string_view second_name,
    std::ostream& output
) {
    git_libgit2_init();

    git_diff_options diff_options = GIT_DIFF_OPTIONS_INIT;

    DiffOutputPayload payload{&output, false};

    int error = git_diff_buffers(
        first_text.data(),
        first_text.size(),
        first_name.data(),
        second_text.data(),
        second_text.size(),
        second_name.data(),
        &diff_options,
        &GitTextDiff::print_diff_file,
        nullptr,
        &GitTextDiff::print_diff_hunk,
        &GitTextDiff::print_diff_line,
        &payload
    );

    git_libgit2_shutdown();

    return {
        .error = error,
        .has_difference = payload.printed,
    };
}
