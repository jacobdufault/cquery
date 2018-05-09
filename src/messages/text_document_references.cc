#include "message_handler.h"
#include "query_utils.h"
#include "queue_manager.h"

#include <loguru.hpp>

namespace {
MethodType kMethodType = "textDocument/references";

struct In_TextDocumentReferences : public RequestInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  struct lsReferenceContext {
    // Include the declaration of the current symbol.
    bool includeDeclaration;
    // Include references with these |Role| bits set.
    Role role = Role::All;
  };
  struct Params {
    lsTextDocumentIdentifier textDocument;
    lsPosition position;
    lsReferenceContext context;
  };

  Params params;
};
MAKE_REFLECT_STRUCT(In_TextDocumentReferences::lsReferenceContext,
                    includeDeclaration,
                    role);
MAKE_REFLECT_STRUCT(In_TextDocumentReferences::Params,
                    textDocument,
                    position,
                    context);
MAKE_REFLECT_STRUCT(In_TextDocumentReferences, id, params);
REGISTER_IN_MESSAGE(In_TextDocumentReferences);

struct Handler_TextDocumentReferences
    : BaseMessageHandler<In_TextDocumentReferences> {
  MethodType GetMethodType() const override { return kMethodType; }

  void Run(In_TextDocumentReferences* request) override {
    QueryFile* file;
    if (!FindFileOrFail(db, project, request->id,
                        request->params.textDocument.uri.GetAbsolutePath(),
                        &file)) {
      return;
    }

    WorkingFile* working_file =
        working_files->GetFileByFilename(file->def->path);

    Out_LocationList out;
    out.id = request->id;

    for (const SymbolRef& sym :
         FindSymbolsAtLocation(working_file, file, request->params.position)) {
      // Found symbol. Return references.
      EachOccurrenceWithParent(
          db, sym, request->params.context.includeDeclaration,
          [&](Use use, lsSymbolKind parent_kind) {
            if (use.role & request->params.context.role)
              if (optional<lsLocation> ls_loc =
                      GetLsLocation(db, working_files, use)) {
                out.result.push_back(*ls_loc);
              }
          });
      break;
    }

    if (out.result.empty())
      for (const IndexInclude& include : file->def->includes)
        if (include.line == request->params.position.line) {
          // |include| is the line the cursor is on.
          for (QueryFile& file1 : db->files)
            if (file1.def)
              for (const IndexInclude& include1 : file1.def->includes)
                if (include1.resolved_path == include.resolved_path) {
                  // Another file |file1| has the same include line.
                  lsLocation result;
                  result.uri = lsDocumentUri::FromPath(file1.def->path);
                  result.range.start.line = result.range.end.line =
                      include1.line;
                  out.result.push_back(std::move(result));
                  break;
                }
          break;
        }

    if ((int)out.result.size() >= g_config->xref.maxNum)
      out.result.resize(g_config->xref.maxNum);
    QueueManager::WriteStdout(kMethodType, out);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentReferences);
}  // namespace
