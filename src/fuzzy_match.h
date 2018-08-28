#pragma once

#include <string_view.h>

#include <limits.h>
#include <string>
#include "completion_matcher.h"

class FuzzyMatcher : public CompletionMatcher {
 public:
  constexpr static int kMaxPat = 100;
  constexpr static int kMaxText = 200;
  // Negative but far from INT_MIN so that intermediate results are hard to
  // overflow.
  constexpr static int kMinScore = INT_MIN / 4;

  FuzzyMatcher(std::string_view pattern);
  int Match(std::string_view text) override;
  int MinScore() const override;

 private:
  std::string_view original_pattern;
  std::string pat;
  std::string_view text;
  int pat_set, text_set;
  char low_pat[kMaxPat], low_text[kMaxText];
  int pat_role[kMaxPat], text_role[kMaxText];
  int dp[2][kMaxText + 1][2];

  int MatchScore(int i, int j, bool last);
  int MissScore(int j, bool last);
};
