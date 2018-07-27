#pragma once

#include <atomic>

struct DiagnosticsEngine;
struct FileConsumerSharedState;
struct ImportManager;
struct Project;
struct QueryDatabase;
struct SemanticHighlightSymbolCache;
struct TimestampManager;
struct WorkingFiles;

// FIXME: rename
struct ImportPipelineStatus {
  std::atomic<int> num_active_threads;
  std::atomic<long long> next_progress_output;

  ImportPipelineStatus();
};

void Indexer_Main(DiagnosticsEngine* diag_engine,
                  FileConsumerSharedState* file_consumer_shared,
                  TimestampManager* timestamp_manager,
                  ImportManager* import_manager,
                  ImportPipelineStatus* status,
                  Project* project,
                  WorkingFiles* working_files);

bool QueryDb_ImportMain(QueryDatabase* db,
                        ImportManager* import_manager,
                        ImportPipelineStatus* status,
                        SemanticHighlightSymbolCache* semantic_cache,
                        WorkingFiles* working_files);
