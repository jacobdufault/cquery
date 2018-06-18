#include "message_handler.h"
#include "query_utils.h"
#include "queue_manager.h"

namespace {
MethodType kMethodType = "$cquery/callers";

struct In_CqueryCallers : public RequestInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  lsTextDocumentPositionParams params;
};
MAKE_REFLECT_STRUCT(In_CqueryCallers, id, params);
REGISTER_IN_MESSAGE(In_CqueryCallers);

struct Handler_CqueryCallers : BaseMessageHandler<In_CqueryCallers> {
  MethodType GetMethodType() const override { return kMethodType; }
  void Run(In_CqueryCallers* request) override {
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
    for (QueryId::SymbolRef sym :
         FindSymbolsAtLocation(working_file, file, request->params.position)) {
      if (sym.kind == SymbolKind::Func) {
        QueryFunc& func = db->GetFunc(sym);
        std::vector<QueryId::LexicalRef> uses = func.uses;
        for (QueryId::LexicalRef func_ref : GetRefsForAllBases(db, func))
          uses.push_back(func_ref);
        for (QueryId::LexicalRef func_ref : GetRefsForAllDerived(db, func))
          uses.push_back(func_ref);
        out.result = GetLsLocations(db, working_files, uses);
        break;
      }
    }
    QueueManager::WriteStdout(kMethodType, out);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_CqueryCallers);
}  // namespace
