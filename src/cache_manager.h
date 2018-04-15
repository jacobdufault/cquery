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

std::string PathToContentKey(Config* config, const NormalizedPath& path);
std::string PathToIndexKey(Config* config, const NormalizedPath& path);

struct ICacheStore 
{
    virtual optional<std::string> Read(const std::string& key) = 0;
    virtual void Write(const std::string& key, const std::string& value) = 0;
    virtual ~ICacheStore() {}
};

// Returns null if the given root path does not exist
std::shared_ptr<ICacheStore> OpenOrConnectFileStore(Config* config, const NormalizedPath& path);

// Return null if the given file path does not exists and cannot be created
std::shared_ptr<ICacheStore> OpenOrConnectUnqliteStore(Config* config, const NormalizedPath& path_to_db);

struct IndexCache
{
public:
    IndexCache(Config* config, std::shared_ptr<ICacheStore> driver);

    // Tries to recover an index file (content+serialized index) for a given source file from the cache store and returns a
    // non-owning reference or null, buffering the IndexFile internally for later take
    IndexFile* TryLoad(const NormalizedPath& path);

    // Tries to recover an index file (content+serialized index) for a given source file from the cache store and returns a
    // non-owning reference or null
    std::unique_ptr<IndexFile> TryTakeOrLoad(const NormalizedPath& path);

    // Only load the buffered file content from the cache
    optional<std::string> TryLoadContent(const NormalizedPath& path);

    // Write an IndexFile to the cache storage
    void Write(IndexFile& file);

    // Iterate over all loaded caches.
    void IterateLoadedCaches(std::function<void(IndexFile*)> fn);

private:
    std::unique_ptr<IndexFile> LoadIndexFileFromCache(const NormalizedPath& path);

    Config* config_;
    std::shared_ptr<ICacheStore> driver_;
    std::unordered_map<std::string, std::unique_ptr<IndexFile>> caches_;    // Using NormalizedPath here would add a dependency to this header
};

std::shared_ptr<IndexCache> MakeIndexCache(Config* config_, std::shared_ptr<ICacheStore> store);



/*
struct ICacheManager {
  struct FakeCacheEntry {
    std::string path;
    std::string content;
    std::string json;
  };

  static std::shared_ptr<ICacheManager> Make();
  static std::shared_ptr<ICacheManager> MakeFake(
      const std::vector<FakeCacheEntry>& entries);

  virtual ~ICacheManager();

  // Tries to load a cache for |path|, returning null if there is none. The
  // cache loader still owns the cache.
  IndexFile* TryLoad(const std::string& path);

  // Takes the existing cache or loads the cache at |path|. May return null if
  // the cache does not exist.
  std::unique_ptr<IndexFile> TryTakeOrLoad(const std::string& path);

  // Takes the existing cache or loads the cache at |path|. Asserts the cache
  // exists.
  std::unique_ptr<IndexFile> TakeOrLoad(const std::string& path);

  virtual void WriteToCache(IndexFile& file) = 0;

  virtual optional<std::string> LoadCachedFileContents(
      const std::string& path) = 0;

  // Iterate over all loaded caches.
  void IterateLoadedCaches(std::function<void(IndexFile*)> fn);

 protected:
  std::unique_ptr<ICacheDriver> driver_;
  //virtual std::unique_ptr<IndexFile> RawCacheLoad(const std::string& path) = 0;
  std::unordered_map<std::string, std::unique_ptr<IndexFile>> caches_;
};
*/
