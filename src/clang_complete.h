#pragma once

#include "atomic_object.h"
#include "clang_index.h"
#include "clang_translation_unit.h"
#include "lru_cache.h"
#include "lsp_completion.h"
#include "lsp_diagnostic.h"
#include "project.h"
#include "threaded_queue.h"
#include "working_files.h"

#include <clang-c/Index.h>

#include <functional>
#include <memory>
#include <mutex>
#include <string>

struct CompletionSession
    : public std::enable_shared_from_this<CompletionSession> {
  // Translation unit for clang.
  struct Tu {
    Tu();

    ClangIndex index;

    // When |tu| was last parsed.
    optional<std::chrono::time_point<std::chrono::high_resolution_clock>>
        last_parsed_at;
    // Acquired when |tu| is being used.
    std::mutex lock;
    std::unique_ptr<ClangTranslationUnit> tu;
  };

  Project::Entry file;
  WorkingFiles* working_files;

  Tu completion;
  Tu diagnostics;

  CompletionSession(const Project::Entry& file, WorkingFiles* working_files);
  ~CompletionSession();
};

struct ClangCompleteManager {
  using OnDiagnostic =
      std::function<void(std::string path,
                         std::vector<lsDiagnostic> diagnostics)>;
  using OnComplete =
      std::function<void(const lsRequestId& id,
                         const std::vector<lsCompletionItem>& results,
                         bool is_cached_result)>;
  using OnDropped = std::function<void(lsRequestId request_id)>;

  struct PreloadRequest {
    PreloadRequest(const AbsolutePath& path);

    std::chrono::time_point<std::chrono::high_resolution_clock> request_time;
    AbsolutePath path;
  };
  struct CompletionRequest {
    CompletionRequest(const lsRequestId& id,
                      const AbsolutePath& path,
                      const lsPosition& position,
                      const OnComplete& on_complete);

    lsRequestId id;
    AbsolutePath path;
    lsPosition position;
    OnComplete on_complete;
  };
  struct DiagnosticRequest {
    DiagnosticRequest(const AbsolutePath& path);

    AbsolutePath path;
  };

  ClangCompleteManager(Project* project,
                       WorkingFiles* working_files,
                       OnDiagnostic on_diagnostic,
                       OnDropped on_dropped);
  ~ClangCompleteManager();

  // Start a code completion at the given location. |on_complete| will run when
  // completion results are available. |on_complete| may run on any thread.
  void CodeComplete(const lsRequestId& request_id,
                    const lsTextDocumentPositionParams& completion_location,
                    const OnComplete& on_complete);
  // Request a diagnostics update.
  void DiagnosticsUpdate(const std::string& path);

  // Notify the completion manager that |filename| has been viewed and we
  // should begin preloading completion data.
  void NotifyView(const AbsolutePath& filename);
  // Notify the completion manager that |filename| has been edited.
  void NotifyEdit(const AbsolutePath& filename);
  // Notify the completion manager that |filename| has been saved. This
  // triggers a reparse.
  void NotifySave(const AbsolutePath& filename);
  // Notify the completion manager that |filename| has been closed. Any existing
  // completion session will be dropped.
  void NotifyClose(const AbsolutePath& filename);

  // Ensures there is a completion or preloaded session. Returns true if a new
  // session was created.
  bool EnsureCompletionOrCreatePreloadSession(const AbsolutePath& filename);
  // Tries to find an edit session for |filename|. This will move the session
  // from view to edit.
  std::shared_ptr<CompletionSession> TryGetSession(const std::string& filename,
                                                   bool mark_as_completion,
                                                   bool create_if_needed);

  // Flushes all saved sessions with the supplied filename
  void FlushSession(const std::string& filename);
  // Flushes all saved sessions
  void FlushAllSessions(void);

  // TODO: make these configurable.
  const int kMaxPreloadedSessions = 10;
  const int kMaxCompletionSessions = 5;

  // Global state.
  Project* project_;
  WorkingFiles* working_files_;
  OnDiagnostic on_diagnostic_;
  OnDropped on_dropped_;

  using LruSessionCache =
      LruCache<std::string, std::shared_ptr<CompletionSession>>;

  // CompletionSession instances which are preloaded, ie, files which the user
  // has viewed but not requested code completion for.
  LruSessionCache preloaded_sessions_;
  // CompletionSession instances which the user has actually performed
  // completion on. This is more rare so these instances tend to stay alive
  // much longer than the ones in |preloaded_sessions_|.
  LruSessionCache completion_sessions_;
  // Mutex which protects |view_sessions_| and |edit_sessions_|.
  std::mutex sessions_lock_;

  // Request a code completion at the given location.
  ThreadedQueue<std::unique_ptr<CompletionRequest>> completion_request_;
  ThreadedQueue<std::unique_ptr<DiagnosticRequest>> diagnostics_request_;
  // Parse requests. The path may already be parsed, in which case it should be
  // reparsed.
  ThreadedQueue<PreloadRequest> preload_requests_;
};
