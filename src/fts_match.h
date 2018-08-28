#pragma once

#include "completion_matcher.h"

class FtsMatcher : public CompletionMatcher {
 public:
  FtsMatcher(std::string_view pattern);
  int Match(std::string_view text) override;
  int MinScore() const override;

 private:
  std::string_view original_pattern;
};
