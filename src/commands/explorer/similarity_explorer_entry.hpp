#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SimilarityExplorerEntry {
  std::string path_a;
  std::string path_b;
  std::string dir_a;
  std::string dir_b;
  std::string filename_a;
  std::string filename_b;
  std::string func_a;
  std::string func_b;
  int start_a;
  int start_b;
  int end_a;
  int end_b;
  int duplicated_lines{-1};
};

inline void to_json(json& j, const SimilarityExplorerEntry& d) {
  j = {
    {"path_a", d.path_a},
    {"path_b", d.path_b},
    {"dir_a", d.dir_a},
    {"dir_b", d.dir_b},
    {"filename_a", d.filename_a},
    {"filename_b", d.filename_b},
    {"func_a", d.func_a},
    {"func_b", d.func_b},
    {"start_a", d.start_a},
    {"start_b", d.start_b},
    {"end_a", d.end_a},
    {"end_b", d.end_b},
    {"duplicated_lines", d.duplicated_lines}
  };
}

typedef struct {
  int found;
  int show;
} SimilarityExplorerInitialMessage;

inline void to_json(json& j, const SimilarityExplorerInitialMessage& d) {
  j = {
    {"found", d.found},
    {"show", d.show},
  };
}