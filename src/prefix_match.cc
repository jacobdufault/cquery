#include "prefix_match.h"

#include <algorithm>
#include <cctype>

namespace {
    bool StartsWith(std::string_view text, std::string_view prefix) {
      return text.find(prefix) == 0;
    }

    bool StartsWithIgnoreCase(std::string_view text, std::string_view prefix) {
      auto min_length = std::min(text.size(), prefix.size());
      if (min_length == 0) {
        return false;
      }
      return std::equal(
          text.begin(), text.begin() + min_length,
          prefix.begin(), prefix.begin() + min_length,
          [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
          );
    }
}

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
