#include "code_complete_cache.h"

void CodeCompleteCache::WithLock(std::function<void()> action) {
  std::lock_guard<std::mutex> lock(mutex_);
  action();
}

bool CodeCompleteCache::IsCacheValid(lsTextDocumentPositionParams position) {
  std::lock_guard<std::mutex> lock(mutex_);
  return cached_path_ == position.textDocument.uri.GetAbsolutePath() &&
         cached_completion_position_ == position.position;
}

void CodeCompleteCache::Clear() {
  std::lock_guard<std::mutex> lock(mutex_);
  cached_path_.reset();
  cached_completion_position_.reset();
  cached_results_.clear();
}