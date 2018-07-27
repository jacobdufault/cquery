#include "import_pipeline.h"

#include "cache_manager.h"
#include "config.h"
#include "diagnostics_engine.h"
#include "iindexer.h"
#include "import_manager.h"
#include "lsp.h"
#include "message_handler.h"
#include "platform.h"
#include "project.h"
#include "query_utils.h"
#include "queue_manager.h"
#include "timer.h"
#include "timestamp_manager.h"

#include <doctest/doctest.h>
#include <loguru.hpp>

#include <atomic>
#include <chrono>
#include <string>
#include <vector>

namespace {

struct Out_Progress : public lsOutMessage<Out_Progress> {
  struct Params {
    int indexRequestCount = 0;
    int doIdMapCount = 0;
    int onIdMappedCount = 0;
    int onIndexedCount = 0;
    int activeThreads = 0;
  };
  std::string method = "$cquery/progress";
  Params params;
};
MAKE_REFLECT_STRUCT(Out_Progress::Params,
                    indexRequestCount,
                    doIdMapCount,
                    onIdMappedCount,
                    onIndexedCount,
                    activeThreads);
MAKE_REFLECT_STRUCT(Out_Progress, jsonrpc, method, params);

// Instead of processing messages forever, we only process upto
// |kIterationSize| messages of a type at one time. While the import time
// likely stays the same, this should reduce overall queue lengths which means
// the user gets a usable index faster.
constexpr int kIterationSize = 200;
struct IterationLoop {
  int remaining = kIterationSize;
  void IncreaseCount() { remaining *= 50; }

  bool Next() { return remaining-- > 0; }
  void Reset() { remaining = kIterationSize; }
};

struct IModificationTimestampFetcher {
  virtual ~IModificationTimestampFetcher() = default;
  virtual optional<int64_t> GetModificationTime(const AbsolutePath& path) = 0;
};
struct RealModificationTimestampFetcher : IModificationTimestampFetcher {
  ~RealModificationTimestampFetcher() override = default;

  // IModificationTimestamp:
  optional<int64_t> GetModificationTime(const AbsolutePath& path) override {
    return GetLastModificationTime(path);
  }
};
struct FakeModificationTimestampFetcher : IModificationTimestampFetcher {
  std::unordered_map<std::string, optional<int64_t>> entries;

  ~FakeModificationTimestampFetcher() override = default;

  // IModificationTimestamp:
  optional<int64_t> GetModificationTime(const AbsolutePath& path) override {
    auto it = entries.find(path);
    assert(it != entries.end());
    return it->second;
  }
};

struct ActiveThread {
  ActiveThread(ImportPipelineStatus* status) : status_(status) {
    if (g_config->progressReportFrequencyMs < 0)
      return;

    ++status_->num_active_threads;
  }
  ~ActiveThread() {
    if (g_config->progressReportFrequencyMs < 0)
      return;

    --status_->num_active_threads;
    EmitProgress();
  }

  // Send indexing progress to client if reporting is enabled.
  void EmitProgress() {
    auto* queue = QueueManager::instance();
    Out_Progress out;
    out.params.indexRequestCount = queue->index_request.Size();
    out.params.doIdMapCount = queue->do_id_map.Size();
    out.params.onIdMappedCount = queue->on_id_mapped.Size();
    out.params.onIndexedCount = queue->on_indexed_for_merge.Size() +
                                queue->on_indexed_for_querydb.Size();
    out.params.activeThreads = status_->num_active_threads;

    // Ignore this progress update if the last update was too recent.
    if (g_config->progressReportFrequencyMs != 0) {
      // Make sure we output a status update if queue lengths are zero.
      bool all_zero =
          out.params.indexRequestCount == 0 && out.params.doIdMapCount == 0 &&
          out.params.onIdMappedCount == 0 && out.params.onIndexedCount == 0 &&
          out.params.activeThreads == 0;
      if (!all_zero &&
          Timer::GetCurrentTimeInMilliseconds() < status_->next_progress_output)
        return;
      status_->next_progress_output =
          Timer::GetCurrentTimeInMilliseconds() + g_config->progressReportFrequencyMs;
    }

    QueueManager::WriteStdout(kMethodType_Unknown, out);
  }

  ImportPipelineStatus* status_;
};

// Checks if |path| needs to be reparsed. This will modify cached state
// such that calling this function twice with the same path may return true
// the first time but will return false the second.
//
// |from|: The file which generated the parse request for this file.
enum class ChangeResult { kYes, kNo, kDeleted };
ChangeResult ComputeChangeStatus(
    TimestampManager* timestamp_manager,
    IModificationTimestampFetcher* modification_timestamp_fetcher,
    const std::shared_ptr<ICacheManager>& cache_manager,
    IndexFile* opt_previous_index,
    const AbsolutePath& path,
    const std::vector<std::string>& args,
    const optional<AbsolutePath>& from) {
  auto unwrap_opt = [](const optional<AbsolutePath>& opt) -> std::string {
    if (opt)
      return " (via " + opt->path + ")";
    return "";
  };
  optional<int64_t> modification_timestamp =
      modification_timestamp_fetcher->GetModificationTime(path);

  // Cannot find file.
  if (!modification_timestamp)
    return ChangeResult::kDeleted;

  optional<int64_t> last_cached_modification =
      timestamp_manager->GetLastCachedModificationTime(cache_manager.get(),
                                                       path);

  // File has been changed.
  if (!last_cached_modification ||
      modification_timestamp != *last_cached_modification) {
    LOG_S(INFO) << "Timestamp has changed for " << path << unwrap_opt(from);
    return ChangeResult::kYes;
  }

  // Command-line arguments changed.
  auto is_file = [](const std::string& arg) {
    return EndsWithAny(arg, {".h", ".c", ".cc", ".cpp", ".hpp", ".m", ".mm"});
  };
  if (opt_previous_index) {
    auto& prev_args = opt_previous_index->args;
    bool same = prev_args.size() == args.size();
    for (size_t i = 0; i < args.size() && same; ++i) {
      same = prev_args[i] == args[i] ||
             (is_file(prev_args[i]) && is_file(args[i]));
    }
    if (!same) {
      LOG_S(INFO) << "Arguments have changed for " << path << unwrap_opt(from);
      return ChangeResult::kYes;
    }
  }

  // File has not changed, do not parse it.
  return ChangeResult::kNo;
}

enum CacheLoadResult { kParse, kDoNotParse };
CacheLoadResult TryLoadFromCache(
    FileConsumerSharedState* file_consumer_shared,
    TimestampManager* timestamp_manager,
    IModificationTimestampFetcher* modification_timestamp_fetcher,
    ImportManager* import_manager,
    const std::shared_ptr<ICacheManager>& cache_manager,
    bool is_interactive,
    const Project::Entry& entry,
    const AbsolutePath& path_to_index) {
  // Always run this block, even if we are interactive, so we can check
  // dependencies and reset files in |file_consumer_shared|.
  IndexFile* previous_index = cache_manager->TryLoad(path_to_index);
  if (!previous_index)
    return CacheLoadResult::kParse;

  // If none of the dependencies have changed and the index is not
  // interactive (ie, requested by a file save), skip parsing and just load
  // from cache.

  // Check timestamps and update |file_consumer_shared|.
  ChangeResult path_state = ComputeChangeStatus(
      timestamp_manager, modification_timestamp_fetcher, cache_manager,
      previous_index, path_to_index, entry.args, previous_index->path);
  if (path_state == ChangeResult::kYes)
    file_consumer_shared->Reset(path_to_index);

  // Target file does not exist on disk, do not emit any indexes.
  // TODO: Dependencies should be reassigned to other files. We can do this by
  // updating the "primary_file" if it doesn't exist. Might not actually be a
  // problem in practice.
  if (path_state == ChangeResult::kDeleted)
    return CacheLoadResult::kDoNotParse;

  bool needs_reparse = is_interactive || path_state == ChangeResult::kYes;

  for (const AbsolutePath& dependency : previous_index->dependencies) {
    assert(!dependency.path.empty());

    if (ComputeChangeStatus(timestamp_manager, modification_timestamp_fetcher,
                       cache_manager, previous_index, dependency, entry.args,
                       previous_index->path) == ChangeResult::kYes) {
      needs_reparse = true;

      // Do not break here, as we need to update |file_consumer_shared| for
      // every dependency that needs to be reparsed.
      file_consumer_shared->Reset(dependency);
    }
  }

  // FIXME: should we still load from cache?
  if (needs_reparse)
    return CacheLoadResult::kParse;

  // No timestamps changed - load directly from cache.
  LOG_S(INFO) << "Skipping parse; no timestamp change for " << path_to_index;

  std::vector<Index_DoIdMap> result;
  result.push_back(Index_DoIdMap(cache_manager->TakeOrLoad(path_to_index),
                                 cache_manager, is_interactive,
                                 false /*write_to_disk*/));
  for (const AbsolutePath& dependency : previous_index->dependencies) {
    // Only load a dependency if it is not already loaded.
    //
    // This is important for perf in large projects where there are lots of
    // dependencies shared between many files.
    if (!file_consumer_shared->Mark(dependency))
      continue;

    LOG_S(INFO) << "Emitting index result for " << dependency << " (via "
                << previous_index->path << ")";

    std::unique_ptr<IndexFile> dependency_index =
        cache_manager->TryTakeOrLoad(dependency);

    // |dependency_index| may be null if there is no cache for it but
    // another file has already started importing it.
    if (!dependency_index)
      continue;

    result.push_back(Index_DoIdMap(std::move(dependency_index), cache_manager,
                                   is_interactive, false /*write_to_disk*/));
  }

  QueueManager::instance()->do_id_map.EnqueueAll(std::move(result),
                                                 false /*priority*/);
  return CacheLoadResult::kDoNotParse;
}

std::vector<FileContents> PreloadFileContents(
    const std::shared_ptr<ICacheManager>& cache_manager,
    const Project::Entry& entry,
    const optional<std::string>& entry_contents,
    const AbsolutePath& path_to_index) {
  // Load file contents for all dependencies into memory. If the dependencies
  // for the file changed we may not end up using all of the files we
  // preloaded. If a new dependency was added the indexer will grab the file
  // contents as soon as possible.
  //
  // We do this to minimize the race between indexing a file and capturing the
  // file contents.
  //
  // TODO: We might be able to optimize perf by only copying for files in
  //       working_files. We can pass that same set of files to the indexer as
  //       well. We then default to a fast file-copy if not in working set.

  // index->file_contents comes from cache, so we need to check if that cache is
  // still valid. if so, we can use it, otherwise we need to load from disk.
  auto get_latest_content = [](const AbsolutePath& path, int64_t cached_time,
                               const std::string& cached) -> std::string {
    optional<int64_t> mod_time = GetLastModificationTime(path);
    if (!mod_time)
      return "";

    if (*mod_time == cached_time)
      return cached;

    optional<std::string> fresh_content = ReadContent(path);
    if (!fresh_content) {
      LOG_S(ERROR) << "Failed to load content for " << path;
      return "";
    }
    return *fresh_content;
  };

  std::vector<FileContents> file_contents;
  if (entry_contents)
    file_contents.push_back(FileContents(entry.filename, *entry_contents));
  cache_manager->IterateLoadedCaches([&](IndexFile* index) {
    if (entry_contents && index->path == entry.filename)
      return;
    file_contents.push_back(FileContents(
        index->path,
        get_latest_content(index->path, index->last_modification_time,
                           index->file_contents)));
  });

  return file_contents;
}

void ParseFile(DiagnosticsEngine* diag_engine,
               WorkingFiles* working_files,
               FileConsumerSharedState* file_consumer_shared,
               TimestampManager* timestamp_manager,
               IModificationTimestampFetcher* modification_timestamp_fetcher,
               ImportManager* import_manager,
               IIndexer* indexer,
               const Index_Request& request,
               const Project::Entry& entry) {
  // If the file is inferred, we may not actually be able to parse that file
  // directly (ie, a header file, which are not listed in the project). If this
  // file is inferred, then try to use the file which originally imported it.
  // FIXME: don't use absolute path
  AbsolutePath path_to_index = entry.filename;
  if (entry.is_inferred) {
    IndexFile* entry_cache = request.cache_manager->TryLoad(entry.filename);
    if (entry_cache)
      path_to_index = entry_cache->import_file;
  }

  // Try to load the file from cache.
  if (TryLoadFromCache(file_consumer_shared, timestamp_manager,
                       modification_timestamp_fetcher, import_manager,
                       request.cache_manager, request.is_interactive, entry,
                       path_to_index) == CacheLoadResult::kDoNotParse) {
    return;
  }

  LOG_S(INFO) << "Parsing " << path_to_index;
  std::vector<FileContents> file_contents = PreloadFileContents(
      request.cache_manager, entry, request.contents, path_to_index);

  std::vector<Index_DoIdMap> result;
  auto indexes = indexer->Index(file_consumer_shared, path_to_index, entry.args,
                                file_contents);

  if (!indexes) {
    if (g_config->index.enabled && request.id.has_value()) {
      Out_Error out;
      out.id = request.id;
      out.error.code = lsErrorCodes::InternalError;
      out.error.message = "Failed to index " + path_to_index.path;
      QueueManager::WriteStdout(kMethodType_Unknown, out);
    }
    return;
  }

  // Add the set of indexes we want to actually import from the index operation.
  for (std::unique_ptr<IndexFile>& new_index : *indexes) {
    // Do not allow a file to be imported twice at the same time.
    PipelineStatus status = import_manager->GetStatus(new_index->path);
    if (status == PipelineStatus::kProcessingInitialImport ||
        status == PipelineStatus::kProcessingUpdate) {
      // TODO: add this file to a queue which will be processed after import is
      // done?
      LOG_S(WARNING) << "Dropping duplicate import request for "
                     << new_index->path;
      continue;
    }

    // Set new import status.
    // FIXME: there is a race condition between checking above and here.
    assert(status == PipelineStatus::kNotSeen ||
           status == PipelineStatus::kImported);
    import_manager->SetStatus(new_index->path,
                              status == PipelineStatus::kNotSeen
                                  ? PipelineStatus::kProcessingInitialImport
                                  : PipelineStatus::kProcessingUpdate);

    // Only emit diagnostics for non-interactive sessions, which makes it easier
    // to identify indexing problems. For interactive sessions, diagnostics are
    // handled by code completion.
    if (!request.is_interactive) {
      diag_engine->Publish(working_files, new_index->path,
                           new_index->diagnostics_);
    }

    // When main thread does IdMap request it will request the previous index if
    // needed.
    LOG_S(INFO) << "Emitting index result for " << new_index->path;
    result.push_back(Index_DoIdMap(std::move(new_index), request.cache_manager,
                                   request.is_interactive,
                                   true /*write_to_disk*/));
  }

  // Load previous index if the file has already been imported so we can do a
  // delta update.
  for (Index_DoIdMap& request : result) {
    PipelineStatus status = import_manager->GetStatus(request.current->path);
    assert(status == PipelineStatus::kProcessingInitialImport ||
           status == PipelineStatus::kProcessingUpdate);

    // Do a delta update if we have already imported this file.
    if (status == PipelineStatus::kProcessingUpdate) {
      request.previous =
          request.cache_manager->TryTakeOrLoad(request.current->path);
      LOG_IF_S(ERROR, !request.previous)
          << "Unable to load previous index for already imported index "
          << request.current->path;
    }
  }

  // Write index to disk if requested.
  for (Index_DoIdMap& request : result) {
    if (request.write_to_disk) {
      LOG_S(INFO) << "Writing cached index to disk for "
                  << request.current->path;
      request.cache_manager->WriteToCache(*request.current);
      timestamp_manager->UpdateCachedModificationTime(
          request.current->path, request.current->last_modification_time);
    }
  }

  QueueManager::instance()->do_id_map.EnqueueAll(std::move(result),
                                                 request.is_interactive);
}

bool IndexMain_DoParse(
    DiagnosticsEngine* diag_engine,
    WorkingFiles* working_files,
    FileConsumerSharedState* file_consumer_shared,
    TimestampManager* timestamp_manager,
    IModificationTimestampFetcher* modification_timestamp_fetcher,
    ImportManager* import_manager,
    IIndexer* indexer) {
  auto* queue = QueueManager::instance();
  optional<Index_Request> request =
      queue->index_request.TryDequeue(true /*priority*/);
  if (!request)
    return false;

  Project::Entry entry;
  entry.filename = request->path;
  entry.args = request->args;
  ParseFile(diag_engine, working_files, file_consumer_shared, timestamp_manager,
            modification_timestamp_fetcher, import_manager, indexer,
            request.value(), entry);
  return true;
}

bool IndexMain_DoCreateIndexUpdate(TimestampManager* timestamp_manager) {
  auto* queue = QueueManager::instance();

  bool did_work = false;
  IterationLoop loop;
  while (loop.Next()) {
    optional<Index_OnIdMapped> response =
        queue->on_id_mapped.TryDequeue(true /*priority*/);
    if (!response)
      return did_work;

    did_work = true;

    IdMap* previous_id_map = nullptr;
    IndexFile* previous_index = nullptr;
    if (response->previous) {
      previous_id_map = response->previous->ids.get();
      previous_index = response->previous->file.get();
    }

    // Build delta update.
    IndexUpdate update =
        IndexUpdate::CreateDelta(previous_id_map, response->current->ids.get(),
                                 previous_index, response->current->file.get());
    LOG_S(INFO) << "Built index update for " << response->current->file->path
                << " (is_delta=" << !!response->previous << ")";

    Index_OnIndexed reply(std::move(update));
    const int kMaxSizeForQuerydb = 1000;
    ThreadedQueue<Index_OnIndexed>& q =
        queue->on_indexed_for_querydb.Size() < kMaxSizeForQuerydb
            ? queue->on_indexed_for_querydb
            : queue->on_indexed_for_merge;
    q.Enqueue(std::move(reply), response->is_interactive /*priority*/);
  }

  return did_work;
}

bool IndexMergeIndexUpdates() {
  // Merge low-priority requests, since priority requests should get serviced
  // by querydb asap.

  auto* queue = QueueManager::instance();
  optional<Index_OnIndexed> root =
      queue->on_indexed_for_merge.TryDequeue(false /*priority*/);
  if (!root)
    return false;

  bool did_merge = false;
  IterationLoop loop;
  while (loop.Next()) {
    optional<Index_OnIndexed> to_join =
        queue->on_indexed_for_merge.TryDequeue(false /*priority*/);
    if (!to_join)
      break;
    did_merge = true;
    root->update.Merge(std::move(to_join->update));
  }

  const int kMaxSizeForQuerydb = 1500;
  ThreadedQueue<Index_OnIndexed>& q =
      queue->on_indexed_for_querydb.Size() < kMaxSizeForQuerydb
          ? queue->on_indexed_for_querydb
          : queue->on_indexed_for_merge;
  q.Enqueue(std::move(*root), false /*priority*/);
  return did_merge;
}

}  // namespace

ImportPipelineStatus::ImportPipelineStatus()
    : num_active_threads(0), next_progress_output(0) {}

void Indexer_Main(DiagnosticsEngine* diag_engine,
                  FileConsumerSharedState* file_consumer_shared,
                  TimestampManager* timestamp_manager,
                  ImportManager* import_manager,
                  ImportPipelineStatus* status,
                  Project* project,
                  WorkingFiles* working_files) {
  RealModificationTimestampFetcher modification_timestamp_fetcher;
  auto* queue = QueueManager::instance();
  // Build one index per-indexer, as building the index acquires a global lock.
  auto indexer = IIndexer::MakeClangIndexer();

  while (true) {
    bool did_work = false;

    {
      ActiveThread active_thread(status);

      // TODO: process all off IndexMain_DoIndex before calling
      // IndexMain_DoCreateIndexUpdate for better icache behavior. We need to
      // have some threads spinning on both though otherwise memory usage will
      // get bad.

      // We need to make sure to run both IndexMain_DoParse and
      // IndexMain_DoCreateIndexUpdate so we don't starve querydb from doing any
      // work. Running both also lets the user query the partially constructed
      // index.
      did_work =
          IndexMain_DoParse(diag_engine, working_files, file_consumer_shared,
                            timestamp_manager, &modification_timestamp_fetcher,
                            import_manager, indexer.get()) ||
          did_work;

      did_work = IndexMain_DoCreateIndexUpdate(timestamp_manager) || did_work;

      // Nothing to index and no index updates to create, so join some already
      // created index updates to reduce work on querydb thread.
      if (!did_work)
        did_work = IndexMergeIndexUpdates() || did_work;
    }

    // We didn't do any work, so wait for a notification.
    if (!did_work) {
      QueueManager::instance()->indexer_waiter->Wait(
          &queue->index_request, &queue->on_id_mapped,
          &queue->load_previous_index, &queue->on_indexed_for_merge);
    }
  }
}

namespace {
void QueryDb_DoIdMap(QueueManager* queue,
                     QueryDatabase* db,
                     ImportManager* import_manager,
                     Index_DoIdMap* request) {
  assert(request->current);
  Index_OnIdMapped response(request->cache_manager, request->is_interactive,
                            request->write_to_disk);
  auto make_map = [db](std::unique_ptr<IndexFile> file)
      -> std::unique_ptr<Index_OnIdMapped::File> {
    if (!file)
      return nullptr;

    auto id_map = std::make_unique<IdMap>(db, file->id_cache);
    return std::make_unique<Index_OnIdMapped::File>(std::move(file),
                                                    std::move(id_map));
  };
  response.current = make_map(std::move(request->current));
  response.previous = make_map(std::move(request->previous));

  queue->on_id_mapped.Enqueue(std::move(response),
                              response.is_interactive /*priority*/);
}

void QueryDb_OnIndexed(QueueManager* queue,
                       QueryDatabase* db,
                       ImportManager* import_manager,
                       ImportPipelineStatus* status,
                       SemanticHighlightSymbolCache* semantic_cache,
                       WorkingFiles* working_files,
                       Index_OnIndexed* response) {
  Timer time;
  db->ApplyIndexUpdate(&response->update);
  time.ResetAndPrint("Applying index update for " +
                     std::to_string(response->update.files_def_update.size()) +
                     " files");

  // Update indexed content, inactive lines, and semantic highlighting.
  for (auto& updated_file : response->update.files_def_update) {
    WorkingFile* working_file =
        working_files->GetFileByFilename(updated_file.value.path);
    if (working_file) {
      // Update indexed content.
      working_file->SetIndexContent(updated_file.file_content);

      // Inactive lines.
      EmitInactiveLines(working_file, updated_file.value.inactive_regions);

      // Semantic highlighting.
      QueryId::File file_id = db->usr_to_file[working_file->filename];
      QueryFile* file = &db->files[file_id.id];
      EmitSemanticHighlighting(db, semantic_cache, working_file, file);
    }

    // TODO/PERF: batch these updates
    // querydb processing of this file has finished.
    import_manager->SetStatus(updated_file.value.path,
                              PipelineStatus::kImported);
  }
}

}  // namespace

bool QueryDb_ImportMain(QueryDatabase* db,
                        ImportManager* import_manager,
                        ImportPipelineStatus* status,
                        SemanticHighlightSymbolCache* semantic_cache,
                        WorkingFiles* working_files) {
  auto* queue = QueueManager::instance();

  ActiveThread active_thread(status);

  bool did_work = false;

  IterationLoop loop;
  loop.IncreaseCount();
  while (loop.Next()) {
    optional<Index_DoIdMap> request =
        queue->do_id_map.TryDequeue(true /*priority*/);
    if (!request)
      break;
    did_work = true;
    QueryDb_DoIdMap(queue, db, import_manager, &*request);
  }

  loop.Reset();
  while (loop.Next()) {
    optional<Index_OnIndexed> response =
        queue->on_indexed_for_querydb.TryDequeue(true /*priority*/);
    if (!response)
      break;
    did_work = true;
    QueryDb_OnIndexed(queue, db, import_manager, status, semantic_cache,
                      working_files, &*response);
  }

  return did_work;
}

TEST_SUITE("ImportPipeline") {
  struct Fixture {
    Fixture() {
      QueueManager::Init();

      queue = QueueManager::instance();
      cache_manager = ICacheManager::MakeFake({});
      indexer = IIndexer::MakeTestIndexer({});
      diag_engine.Init();
    }

    bool PumpOnce() {
      return IndexMain_DoParse(&diag_engine, &working_files,
                               &file_consumer_shared, &timestamp_manager,
                               &modification_timestamp_fetcher, &import_manager,
                               indexer.get());
    }

    void MakeRequest(const std::string& path,
                     const std::vector<std::string>& args = {},
                     bool is_interactive = false,
                     const std::string& contents = "void foo();") {
      queue->index_request.Enqueue(
          Index_Request(path, args, is_interactive, contents, cache_manager),
          false /*priority*/);
    }

    QueueManager* queue = nullptr;
    DiagnosticsEngine diag_engine;
    WorkingFiles working_files;
    FileConsumerSharedState file_consumer_shared;
    TimestampManager timestamp_manager;
    FakeModificationTimestampFetcher modification_timestamp_fetcher;
    ImportManager import_manager;
    std::shared_ptr<ICacheManager> cache_manager;
    std::unique_ptr<IIndexer> indexer;
  };

  TEST_CASE_FIXTURE(Fixture, "FileNeedsParse") {
    auto check = [&](const std::string& file, bool is_dependency = false,
                     bool is_interactive = false,
                     const std::vector<std::string>& old_args = {},
                     const std::vector<std::string>& new_args = {}) {
      std::unique_ptr<IndexFile> opt_previous_index;
      if (!old_args.empty()) {
        opt_previous_index = std::make_unique<IndexFile>(
            AbsolutePath("---.cc", false /*validate*/), "<empty>");
        opt_previous_index->args = old_args;
      }
      optional<AbsolutePath> from;
      if (is_dependency)
        from = AbsolutePath("---.cc", false /*validate*/);
      return ComputeChangeStatus(
          &timestamp_manager, &modification_timestamp_fetcher, cache_manager,
          opt_previous_index.get(), AbsolutePath(file, false /*validate*/),
          new_args, from);
    };

    // A file with no timestamp is not imported, since this implies the file no
    // longer exists on disk.
    modification_timestamp_fetcher.entries["bar.h"] = nullopt;
    REQUIRE(check("bar.h", false /*is_dependency*/) == ChangeResult::kDeleted);

    // A file whose timestamp has not changed is not imported. When the
    // timestamp changes (either forward or backward) it is reimported.
    auto check_timestamp_change = [&](int64_t timestamp) {
      modification_timestamp_fetcher.entries["aa.cc"] = timestamp;
      REQUIRE(check("aa.cc") == ChangeResult::kYes);
      REQUIRE(check("aa.cc") == ChangeResult::kYes);
      REQUIRE(check("aa.cc") == ChangeResult::kYes);
      timestamp_manager.UpdateCachedModificationTime("aa.cc", timestamp);
      REQUIRE(check("aa.cc") == ChangeResult::kNo);
    };
    check_timestamp_change(5);
    check_timestamp_change(6);
    check_timestamp_change(5);
    check_timestamp_change(4);

    // Argument change implies reimport, even if timestamp has not changed.
    timestamp_manager.UpdateCachedModificationTime("aa.cc", 5);
    modification_timestamp_fetcher.entries["aa.cc"] = 5;
    REQUIRE(check("aa.cc", false /*is_dependency*/, false /*is_interactive*/,
                  {"b"} /*old_args*/,
                  {"b", "a"} /*new_args*/) == ChangeResult::kYes);
  }

  // FIXME: validate other state like timestamp_manager, etc.
  // FIXME: add more interesting tests that are not the happy path
  // FIXME: test
  //   - IndexMain_DoCreateIndexUpdate
  //   - QueryDb_ImportMain

  TEST_CASE_FIXTURE(Fixture, "index request with zero results") {
    indexer = IIndexer::MakeTestIndexer({IIndexer::TestEntry{"foo.cc", 0}});

    MakeRequest("foo.cc");

    REQUIRE(queue->index_request.Size() == 1);
    REQUIRE(queue->do_id_map.Size() == 0);
    PumpOnce();
    REQUIRE(queue->index_request.Size() == 0);
    REQUIRE(queue->do_id_map.Size() == 0);

    REQUIRE(file_consumer_shared.used_files.empty());
  }

  TEST_CASE_FIXTURE(Fixture, "one index request") {
    indexer = IIndexer::MakeTestIndexer({IIndexer::TestEntry{"foo.cc", 100}});

    MakeRequest("foo.cc");

    REQUIRE(queue->index_request.Size() == 1);
    REQUIRE(queue->do_id_map.Size() == 0);
    PumpOnce();
    REQUIRE(queue->index_request.Size() == 0);
    REQUIRE(queue->do_id_map.Size() == 100);

    REQUIRE(file_consumer_shared.used_files.empty());
  }

  TEST_CASE_FIXTURE(Fixture, "multiple index requests") {
    indexer = IIndexer::MakeTestIndexer(
        {IIndexer::TestEntry{"foo.cc", 100}, IIndexer::TestEntry{"bar.cc", 5}});

    MakeRequest("foo.cc");
    MakeRequest("bar.cc");

    REQUIRE(queue->index_request.Size() == 2);
    REQUIRE(queue->do_id_map.Size() == 0);
    while (PumpOnce()) {
    }
    REQUIRE(queue->index_request.Size() == 0);
    REQUIRE(queue->do_id_map.Size() == 105);

    REQUIRE(file_consumer_shared.used_files.empty());
  }
}
