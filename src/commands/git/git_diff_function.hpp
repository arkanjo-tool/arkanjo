#pragma once

#include <string>
#include <vector>

#include <git2.h>

#include <arkanjo/base/path.hpp>
#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/commands/command_base.hpp>

/**
 * @brief Generate a git-style diff between two duplicate functions.
 *
 * This command matches two function name patterns against the similarity
 * table, validates that the functions are considered duplicates, and then
 * uses libgit2's diff engine to render a patch between the two function
 * bodies.
 */
class GitDiffFunction : public CommandBase<GitDiffFunction> {
  public:
    static constexpr CliOption options_[] = {
        {"first_function", 0, PositionalArgument, "First function name pattern."},
        {"second_function", 0, PositionalArgument, "Second function name pattern."},
        OPTION_END
    };
    COMMAND_DESCRIPTION("Show a git-style diff for two duplicate functions found by the tool.")

    explicit GitDiffFunction(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options) override;
    bool run(const ParsedOptions& options) override;

  private:
    Similarity_Table* similarity_table;

    std::vector<Path> find_matching_paths(const std::string& pattern) const;
    const std::string format_pattern(const std::string& pattern) const;
    Path choose_single_path(const std::vector<Path>& candidates, const std::string& pattern) const;
    bool diff_functions(const Path& first, const Path& second) const;
    static int print_diff_line(const git_diff_delta* delta,
                               const git_diff_hunk* hunk,
                               const git_diff_line* line,
                               void* payload);
    static std::string build_text_from_lines(const std::vector<std::string>& lines);
};
