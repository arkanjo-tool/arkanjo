#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef struct {
  std::string pattern;
} SimilarFunctionEmptyEntry;

inline void to_json(json& j, const SimilarFunctionEmptyEntry& d) {
  j = {
    {"pattern", d.pattern},
  };
}

typedef struct {
  size_t count;
} SimilarFunctionCountEntry;

inline void to_json(json& j, const SimilarFunctionCountEntry& d) {
  j = {
    {"count", d.count},
  };
}