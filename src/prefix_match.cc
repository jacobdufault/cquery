#include "prefix_match.h"

#include "utils.h"
#include <algorithm>

PrefixMatcher::PrefixMatcher(std::string_view pattern, bool case_sensitive) {
  original_pattern = pattern;
  this->case_sensitive = case_sensitive;
}

int PrefixMatcher::Match(std::string_view text) {
  if (case_sensitive) {
    return ::StartsWith(text, original_pattern) ? 1 : MinScore();
  } else {
    return ::StartsWithIgnoreCase(text, original_pattern) ? 1 : MinScore();
  }
}

int PrefixMatcher::MinScore() const {
  return -1;
}
