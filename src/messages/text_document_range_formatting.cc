#include "clang_format.h"
#include "lex_utils.h"
#include "message_handler.h"
#include "queue_manager.h"
#include "working_files.h"

#include <loguru.hpp>

namespace {
MethodType kMethodType = "textDocument/rangeFormatting";

struct lsTextDocumentRangeFormattingParams {
  lsTextDocumentIdentifier textDocument;
  lsRange range;
  lsFormattingOptions options;
};
MAKE_REFLECT_STRUCT(lsTextDocumentRangeFormattingParams,
                    textDocument,
                    range,
                    options);

struct In_TextDocumentRangeFormatting : public RequestInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  lsTextDocumentRangeFormattingParams params;
};
MAKE_REFLECT_STRUCT(In_TextDocumentRangeFormatting, id, params);
REGISTER_IN_MESSAGE(In_TextDocumentRangeFormatting);

struct Out_TextDocumentRangeFormatting
    : public lsOutMessage<Out_TextDocumentRangeFormatting> {
  lsRequestId id;
  std::vector<lsTextEdit> result;
};
MAKE_REFLECT_STRUCT(Out_TextDocumentRangeFormatting, jsonrpc, id, result);

struct Handler_TextDocumentRangeFormatting
    : BaseMessageHandler<In_TextDocumentRangeFormatting> {
  MethodType GetMethodType() const override { return kMethodType; }

  void Run(In_TextDocumentRangeFormatting* request) override {
    Out_TextDocumentRangeFormatting response;
    response.id = request->id;

    WorkingFile* working_file = working_files->GetFileByFilename(
        request->params.textDocument.uri.GetAbsolutePath());

    int start_offset = GetOffsetForPosition(request->params.range.start,
                                            working_file->buffer_content);
    int end_offset = GetOffsetForPosition(request->params.range.end,
                                          working_file->buffer_content);
    response.result =
        RunClangFormat(working_file->filename, working_file->buffer_content,
                       start_offset, end_offset);

    QueueManager::WriteStdout(kMethodType, response);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentRangeFormatting);
}  // namespace
