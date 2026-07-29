#pragma once

#include <git2.h>

#include <string_view>
#include <iostream>

struct DiffResult {
    int error;
    bool has_difference;
};

class GitTextDiff {
  public:
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
