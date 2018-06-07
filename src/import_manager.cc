#include "import_manager.h"

bool ImportManager::IsInitialImport(const std::string& path) {
  // Try reading the value
  {
    std::shared_lock<std::shared_timed_mutex> lock(initial_import_mutex_);
    if (initial_import_.find(path) != initial_import_.end())
      return false;
  }
  
  // Try inserting the value
  {
    std::unique_lock<std::shared_timed_mutex> lock(initial_import_mutex_);
    return initial_import_.insert(path).second;
  }
}

bool ImportManager::TryMarkDependencyImported(const std::string& path) {
  // Try reading the value
  {
    std::shared_lock<std::shared_timed_mutex> lock(dependency_mutex_);
    if (dependency_imported_.find(path) != dependency_imported_.end())
      return false;
  }

  // Try inserting the value
  {
    std::unique_lock<std::shared_timed_mutex> lock(dependency_mutex_);
    return dependency_imported_.insert(path).second;
  }
}

bool ImportManager::StartQueryDbImport(const std::string& path) {
  return querydb_processing_.insert(path).second;
}

void ImportManager::DoneQueryDbImport(const std::string& path) {
  querydb_processing_.erase(path);
}
