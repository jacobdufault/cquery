#pragma once

#include <iosfwd>
#include <shared_mutex>
#include <string>
#include <unordered_map>

enum class PipelineStatus {
  // The file is has not been processed by the import pipeline in any way.
  kNotSeen,
  // The file is currently in the pipeline but has not been added to querydb
  // yet.
  kProcessingInitialImport,
  // The file is imported, but not currently in the pipeline.
  kImported,
  // The file is imported and also being updated, ie, it is currently in the
  // pipeline.
  kProcessingUpdate
};
std::ostream& operator<<(std::ostream& os, const PipelineStatus& status);

// Manages files inside of the indexing pipeline so we don't have the same file
// being imported multiple times.
struct ImportManager {
  PipelineStatus GetStatus(const std::string& path);
  void SetStatus(const std::string& path, PipelineStatus status);

  // TODO: use shared_mutex
  std::shared_timed_mutex status_mutex_;
  std::unordered_map<std::string, PipelineStatus> status_;
};