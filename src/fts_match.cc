#include "fts_match.h"

#define FTS_FUZZY_MATCH_IMPLEMENTATION 1
#include <fts_fuzzy_match.h>

FtsMatcher::FtsMatcher(std::string_view pattern) {
  original_pattern = pattern;
}

int FtsMatcher::Match(std::string_view text) {
  int result = 0;
  if (fts::fuzzy_match(original_pattern.data(), text.data(), result)) {
    return result;
  }
  return MinScore();
}

int FtsMatcher::MinScore() const {
  return -100000;
}
