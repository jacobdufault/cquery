#pragma once

#include <string_view.h>

class CompletionMatcher {
 public:
  // virtual ~CompletionMatcher() = default; // don't know why but it crashed if I uncomment this like, investigating
  virtual int Match(std::string_view text) = 0;
  virtual int MinScore() const = 0;
};
