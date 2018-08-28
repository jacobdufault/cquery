#pragma once

#include "completion_matcher.h"

class PrefixMatcher : public CompletionMatcher {
 public:
  PrefixMatcher(std::string_view pattern, bool case_sensitive);
  int Match(std::string_view text) override;
  int MinScore() const override;

 private:
  std::string_view original_pattern;
  bool case_sensitive;
};
