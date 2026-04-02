#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef struct {
  std::string path_a;
  std::string path_b;
  double similarity;
} RandomSelectorEntry;

inline void to_json(json& j, const RandomSelectorEntry& d) {
  j = {
    {"path_a", d.path_a},
    {"path_b", d.path_b},
    {"similarity", d.similarity},
  };
}