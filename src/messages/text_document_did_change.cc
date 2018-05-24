#include "cache_manager.h"
#include "clang_complete.h"
#include "message_handler.h"
#include "project.h"
#include "queue_manager.h"
#include "working_files.h"

#include <loguru/loguru.hpp>

namespace {
MethodType kMethodType = "textDocument/didChange";

struct In_TextDocumentDidChange : public NotificationInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  lsTextDocumentDidChangeParams params;
};

MAKE_REFLECT_STRUCT(In_TextDocumentDidChange, params);
REGISTER_IN_MESSAGE(In_TextDocumentDidChange);

struct Handler_TextDocumentDidChange
    : BaseMessageHandler<In_TextDocumentDidChange> {
  MethodType GetMethodType() const override { return kMethodType; }

  void Run(In_TextDocumentDidChange* request) override {
    AbsolutePath path = request->params.textDocument.uri.GetAbsolutePath();
    working_files->OnChange(request->params);
    if (g_config->enableIndexOnDidChange) {
      WorkingFile* working_file = working_files->GetFileByFilename(path);
      Project::Entry entry = project->FindCompilationEntryForFile(path);
      QueueManager::instance()->index_request.Enqueue(
          Index_Request(entry.filename, entry.args, true /*is_interactive*/,
                        working_file->buffer_content, ICacheManager::Make()),
          true /*priority*/);
    }
    clang_complete->NotifyEdit(path);
    clang_complete->DiagnosticsUpdate(path);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentDidChange);
}  // namespace
