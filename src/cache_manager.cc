#include "cache_manager.h"

#include "config.h"
#include "indexer.h"
#include "lsp.h"
#include "platform.h"

#include <loguru/loguru.hpp>

#include <algorithm>
#include <unordered_map>

#include "query.h"
#include "unqlite.h"

namespace {

// Storing index+content in a file directory (possibly shared between multiple
// cquery caches, since it could be a user-setting) The key already has to
// contain the distinction between content and index
struct FileBasedCacheDriver : public ICacheStore {
  FileBasedCacheDriver(Config* config,
                       std::string projectDir,
                       std::string externalsDir)
      : projectDir_(projectDir), externalsDir_(externalsDir) {}

  std::string KeyToFilePath(const std::string& key) {
    assert(!g_config->cacheDirectory.empty());
    std::string cache_file;

    size_t len = g_config->projectRoot.size();
    if (StartsWith(key, g_config->projectRoot)) {
      cache_file = projectDir_ + '/' + EscapeFileName(key.substr(len));
    } else {
      cache_file = externalsDir_ + '/' + EscapeFileName(key);
    }

    return g_config->cacheDirectory + cache_file;
  }

  optional<std::string> Read(const std::string& key) override {
    std::string file_path = KeyToFilePath(key);
    return ReadContent(file_path);
  }

  void Write(const std::string& key, const std::string& value) override {
    std::string file_path = KeyToFilePath(key);
    WriteToFile(file_path, value);
  }

 private:
  std::string projectDir_;
  std::string externalsDir_;
};

void UnqliteHandleResult(std::string operation, unqlite* database, int ret) {
  if (ret != UNQLITE_OK) {
    const char* zBuf;
    int iLen;
    unqlite_config(database, UNQLITE_CONFIG_ERR_LOG, &zBuf, &iLen);
    LOG_S(WARNING) << "Unqlite error: \"" << std::string(zBuf, zBuf + iLen) << "\".";

    switch(ret) {
    case UNQLITE_IOERR:
    case UNQLITE_NOMEM:
        LOG_S(ERROR) << "Rolling back the last commit.";
        unqlite_rollback(database);
    }
  }
}

// Storing index+content in an unqlite database (possibly shared between
// multiple cquery caches, since it could be a user-setting)
struct UnqliteCacheDriver : public ICacheStore {
  UnqliteCacheDriver(unqlite* database) : database_(database), bytesSinceCommit_(0) {}

  UnqliteCacheDriver(UnqliteCacheDriver&) = delete;

  optional<std::string> Read(const std::string& key) override {
    unqlite_int64 valueLength;
    std::string result;
    int ret = unqlite_kv_fetch(database_, key.data(), key.size(), nullptr,
                               &valueLength);

    if (ret == UNQLITE_OK) {
      result.resize(valueLength);
      ret = unqlite_kv_fetch(database_, key.data(), key.size(),
                             const_cast<char*>(result.data()), &valueLength);
    }

    if (ret == UNQLITE_OK)
    {
      LOG_S(INFO) << "unqlite: Handing out cache for key \"" << key << "\"";
      return std::move(result);
    }
    else
    {
      LOG_S(WARNING) << "unqlite: No data for key \"" << key << "\"";
      return {};
    }
  }

  void Write(const std::string& key, const std::string& value) override {
    int ret;
    while ((ret = unqlite_kv_store(database_, key.data(), key.size(),
                                   value.data(), value.size())) == UNQLITE_BUSY)
      ;
    if (ret != UNQLITE_OK) {
      UnqliteHandleResult("unqlite_kv_store", database_, ret);
    }
    else
    {
        bytesSinceCommit_ += value.size();

        if (bytesSinceCommit_ > 16*1024*1024)
        {
            ret = unqlite_commit(database_);
            UnqliteHandleResult("unqlite_commit", database_, ret);
            if (ret == UNQLITE_OK) bytesSinceCommit_ = 0u;
        }
    }
  }

  void Close() override {
    LOG_S(INFO) << "Unqlite: Closing the store.";
    int ret;
    while ((ret = unqlite_close(database_)) == UNQLITE_BUSY)
      ;

    if (ret != UNQLITE_OK) {
      UnqliteHandleResult("unqlite_close", database_, ret);
    }
  }

  ~UnqliteCacheDriver() override {}

  size_t bytesSinceCommit_;
  unqlite* database_;
};

std::string SerializationFormatToSuffix(SerializeFormat format) {
  switch (format) {
    case SerializeFormat::Json:
      return ".json";
    case SerializeFormat::MessagePack:
      return ".mpack";
  }
  assert(false);
  return ".json";
}

}  // namespace

IndexCache::IndexCache(std::shared_ptr<ICacheStore> driver)
    : driver_(std::move(driver)) {}


std::unique_ptr<IndexFile> IndexCache::LoadIndexFileFromCache(
    const NormalizedPath& file) {
  optional<std::string> file_content = driver_->Read(file.path);
  optional<std::string> serialized_indexed_content = driver_->Read(
      file.path + SerializationFormatToSuffix(g_config->cacheFormat));

  if (!file_content || !serialized_indexed_content)
  {
    LOG_S(WARNING) << "IndexCache::LoadIndexFileFromCache: Cannot serve cache request for \"" << file.path << "\"";
    return nullptr;
  }

  return Deserialize(g_config->cacheFormat, file.path,
                     *serialized_indexed_content, *file_content,
                     IndexFile::kMajorVersion);
}

// Tries to recover an index file (content+serialized index) for a given source
// file from the cache store and returns a non-owning reference or null,
// buffering the IndexFile internally for later take
IndexFile* IndexCache::TryLoad(const NormalizedPath& path) {
  IndexFile* result = nullptr;
  auto it = caches_.find(path.path);
  if (it != caches_.end())
    return it->second.get();

  auto ptr = LoadIndexFileFromCache(path);
  if (ptr) {
    result = ptr.get();
    caches_.emplace(path.path, std::move(ptr));
  }
  else
  {
    LOG_S(WARNING) << "IndexCache::TryLoad: Cannot serve cache request for \"" << path.path << "\"";
  }

  return result;
}

// Tries to recover an index file (content+serialized index) for a given source
// file from the cache store and returns a non-owning reference or null
std::unique_ptr<IndexFile> IndexCache::TryTakeOrLoad(
    const NormalizedPath& path) {
  auto it = caches_.find(path.path);
  if (it != caches_.end())
  {
    auto result = std::move(it->second);
    caches_.erase(it);
    return result;
  }

  return LoadIndexFileFromCache(path);
}

// Only load the buffered file content from the cache
optional<std::string> IndexCache::TryLoadContent(const NormalizedPath& path) {
  return driver_->Read(path.path);
}

// Write an IndexFile to the cache storage
void IndexCache::Write(IndexFile& file) {
  driver_->Write(file.path, file.file_contents);
  driver_->Write(file.path.path + SerializationFormatToSuffix(g_config->cacheFormat),
                 Serialize(g_config->cacheFormat, file));
}

// Iterate over all loaded caches.
void IndexCache::IterateLoadedCaches(std::function<void(IndexFile*)> fn) {
  for (auto& file : caches_) {
    fn(file.second.get());
  }
}

std::shared_ptr<IndexCache> MakeIndexCache(std::shared_ptr<ICacheStore> store) {
  return std::make_shared<IndexCache>(std::move(store));
}

// Returns null if the given root path does not exist
std::shared_ptr<ICacheStore> OpenOrConnectFileStore(
    const NormalizedPath& path) {
  const std::string projectDirName = EscapeFileName(g_config->projectRoot);
  const std::string externalsDirName =
      '@' + EscapeFileName(g_config->projectRoot);

  MakeDirectoryRecursive(g_config->cacheDirectory + projectDirName);
  MakeDirectoryRecursive(g_config->cacheDirectory + externalsDirName);

  LOG_S(INFO) << "Connecting to file storage in directory \"" << path.path
              << "\".";

  return std::make_shared<FileBasedCacheDriver>(g_config, projectDirName,
                                                externalsDirName);
}

// Return null if the given file path does not exists and cannot be created
std::shared_ptr<ICacheStore> OpenOrConnectUnqliteStore(
    const NormalizedPath& path_to_db) {
  std::string databaseFile =
      g_config->cacheDirectory + EscapeFileName(g_config->projectRoot) + ".db";
  unqlite* database = nullptr;

  LOG_S(INFO) << "Connecting to unqlite storage in directory \"" << databaseFile
              << "\".";

  int ret = unqlite_open(&database, databaseFile.c_str(), UNQLITE_OPEN_CREATE);
  UnqliteHandleResult("unqlite_open", database, ret);

  // if (ret == UNQLITE_OK) return
  // std::make_shared<UnqliteCacheDriver>(database);
  if (ret == UNQLITE_OK)
    return std::shared_ptr<ICacheStore>(new UnqliteCacheDriver(database));
  else
    return nullptr;
}
