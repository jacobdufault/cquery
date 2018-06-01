#pragma once

// FIXME: do not include clang-c outside of clang_ files.
#include <clang-c/Index.h>

#include <atomic>
#include <memory>
#include <string>
#include <vector>

struct AbsolutePath;
struct ClangTranslationUnit;
struct Config;
class DiagnosticsEngine;
struct FileConsumerSharedState;
struct ImportManager;
struct Project;
struct QueryDatabase;
struct SemanticHighlightSymbolCache;
struct TimestampManager;
struct WorkingFiles;

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
