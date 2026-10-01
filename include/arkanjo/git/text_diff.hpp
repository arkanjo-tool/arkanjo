#pragma once

#include <git2.h>

#include <string_view>
#include <iostream>

/**
 * @brief Represents the outcome of a text diff comparison.
 */
struct DiffResult {
    int error;           ///< Libgit2 error code (0 on success).
    bool has_difference; ///< True if text contents differ.
};

/**
 * @brief Computes and formats line-by-line unified text diffs using libgit2.
 */
class GitTextDiff {
  public:
    /**
     * @brief Compares two strings of text and prints unified diff output to the specified stream.
     * @param first_text Old text buffer.
     * @param first_name Identifier/label for old text.
     * @param second_text New text buffer.
     * @param second_name Identifier/label for new text.
     * @param output Output stream for formatted diff.
     * @return DiffResult indicating success and whether differences exist.
     */
    static DiffResult compare(
        std::string_view first_text,
        std::string_view first_name,
        std::string_view second_text,
        std::string_view second_name,
        std::ostream& output
    );

  private:
    static int print_diff_file(const git_diff_delta* delta, float, void* payload);

    static int print_diff_hunk(const git_diff_delta*,const git_diff_hunk* hunk,void* payload);

    static int print_diff_line(
      const git_diff_delta* /*delta*/,
      const git_diff_hunk* /*hunk*/,
      const git_diff_line* line,
      void* payload
    );
};
