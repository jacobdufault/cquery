#include "clang_format.h"
#include "message_handler.h"
#include "platform.h"
#include "queue_manager.h"
#include "working_files.h"

#include <doctest/doctest.h>
#include <loguru.hpp>
#include <pugixml.hpp>

namespace {
MethodType kMethodType = "textDocument/formatting";

struct In_TextDocumentFormatting : public RequestInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  struct Params {
    lsTextDocumentIdentifier textDocument;
    lsFormattingOptions options;
  };
  Params params;
};
MAKE_REFLECT_STRUCT(In_TextDocumentFormatting::Params, textDocument, options);
MAKE_REFLECT_STRUCT(In_TextDocumentFormatting, id, params);
REGISTER_IN_MESSAGE(In_TextDocumentFormatting);

struct Out_TextDocumentFormatting
    : public lsOutMessage<Out_TextDocumentFormatting> {
  lsRequestId id;
  std::vector<lsTextEdit> result;
};
MAKE_REFLECT_STRUCT(Out_TextDocumentFormatting, jsonrpc, id, result);

struct Handler_TextDocumentFormatting
    : BaseMessageHandler<In_TextDocumentFormatting> {
  MethodType GetMethodType() const override { return kMethodType; }
  void Run(In_TextDocumentFormatting* request) override {
    Out_TextDocumentFormatting response;
    response.id = request->id;

    WorkingFile* working_file = working_files->GetFileByFilename(
        request->params.textDocument.uri.GetAbsolutePath());
    response.result =
        RunClangFormat(working_file->filename, working_file->buffer_content,
                       nullopt /*start_offset*/, nullopt /*end_offset*/);

    QueueManager::WriteStdout(kMethodType, response);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentFormatting);
}  // namespace
