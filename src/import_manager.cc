#include "import_manager.h"

#include "assert.h"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const PipelineStatus& status) {
  switch (status) {
    case PipelineStatus::kNotSeen:
      os << "kNotSeen";
      break;
    case PipelineStatus::kProcessingInitialImport:
      os << "kProcessingInitialImport";
      break;
    case PipelineStatus::kImported:
      os << "kImported";
      break;
    case PipelineStatus::kProcessingUpdate:
      os << "kProcessingUpdate";
      break;
    default:
      assert(false);
  }
  return os;
}

PipelineStatus ImportManager::GetStatus(const std::string& path) {
  // Try reading the value
  {
    std::shared_lock<std::shared_timed_mutex> lock(status_mutex_);
    auto it = status_.find(path);
    if (it != status_.end())
      return it->second;
  }
  return PipelineStatus::kNotSeen;
}