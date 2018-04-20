#pragma once

#include <optional.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query.h"

struct NormalizedPath;
struct Config;
struct IndexFile;

struct ICacheStore;

std::string PathToContentKey(const NormalizedPath& path);
std::string PathToIndexKey(const NormalizedPath& path);

struct ICacheStore {
  virtual optional<std::string> Read(const std::string& key) = 0;
  virtual void Write(const std::string& key, const std::string& value) = 0;
  virtual void Close() {}
  virtual ~ICacheStore() {}
};

// Returns null if the given root path does not exist
std::shared_ptr<ICacheStore> OpenOrConnectFileStore(const NormalizedPath& path);

// Return null if the given file path does not exists and cannot be created
std::shared_ptr<ICacheStore> OpenOrConnectUnqliteStore(
    const NormalizedPath& path_to_db);


// Note that IndexCaches are _not_ thread-safe.
struct IndexCache {
 public:
  IndexCache(std::shared_ptr<ICacheStore> driver);

  // Tries to recover an index file (content+serialized index) for a given
  // source file from the cache store and returns a non-owning reference or
  // null, buffering the IndexFile internally for later take
  IndexFile* TryLoad(const NormalizedPath& path);

  // Tries to recover an index file (content+serialized index) for a given
  // source file from the cache store and returns a non-owning reference or null
  std::unique_ptr<IndexFile> TryTakeOrLoad(const NormalizedPath& path);

  // Only load the buffered file content from the cache
  optional<std::string> TryLoadContent(const NormalizedPath& path);

  // Write an IndexFile to the cache storage
  void Write(IndexFile& file);

  // Iterate over all loaded caches.
  void IterateLoadedCaches(std::function<void(IndexFile*)> fn);

 private:
  std::unique_ptr<IndexFile> LoadIndexFileFromCache(const NormalizedPath& path);

  std::shared_ptr<ICacheStore> driver_;
  std::unordered_map<std::string, std::unique_ptr<IndexFile>>
      caches_;  // Using NormalizedPath here would add a dependency to this
                // header
};

std::shared_ptr<IndexCache> MakeIndexCache(std::shared_ptr<ICacheStore> store);
