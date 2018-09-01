#pragma once

#include <string_view.h>

class CompletionMatcher {
 public:
  virtual ~CompletionMatcher() = default;
  virtual int Match(std::string_view text) = 0;
  virtual int MinScore() const = 0;
};
