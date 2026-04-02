#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef struct {
  std::string path_a;
  std::string path_b;
  int duplicated_lines = -1;
} SimilarityExplorerEntry;

inline void to_json(json& j, const SimilarityExplorerEntry& d) {
  j = {
    {"path_a", d.path_a},
    {"path_b", d.path_b},
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