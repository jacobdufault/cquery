#include "message_handler.h"
#include "query_utils.h"
#include "queue_manager.h"

namespace {
MethodType kMethodType = "textDocument/hover";

// Find the comments for |sym|, if any.
optional<lsMarkedString> GetComments(QueryDatabase* db, SymbolRef sym) {
  auto make = [](std::string_view comment) -> optional<lsMarkedString> {
    lsMarkedString result;
    result.value = comment;
    return result;
  };

  switch (sym.kind) {
    case SymbolKind::Type: {
      if (const auto* def = db->GetType(sym).AnyDef()) {
        if (!def->comments.empty())
          return make(def->comments);
      }
      return nullopt;
    }
    case SymbolKind::Func: {
      if (const auto* def = db->GetFunc(sym).AnyDef()) {
        if (!def->comments.empty())
          return make(def->comments);
      }
      return nullopt;
    }
    case SymbolKind::Var: {
      if (const auto* def = db->GetVar(sym).AnyDef()) {
        if (!def->comments.empty())
          return make(def->comments);
      }
      return nullopt;
    }
    case SymbolKind::File:
    case SymbolKind::Invalid: {
      CQUERY_UNREACHABLE();
      break;
    }
  }
  CQUERY_UNREACHABLE();
  return nullopt;
}

// Returns the hover or detailed name for `sym`, if any.
optional<lsMarkedString> GetHoverOrName(QueryDatabase* db,
                                        const std::string& language,
                                        SymbolRef sym) {
  auto make = [&](std::string_view comment) {
    lsMarkedString result;
    result.language = language;
    result.value = comment;
    return result;
  };

  switch (sym.kind) {
    case SymbolKind::Type: {
      if (const auto* def = db->GetType(sym).AnyDef()) {
        if (!def->hover.empty())
          return make(def->hover);
        if (!def->detailed_name.empty())
          return make(def->detailed_name);
      }
      return nullopt;
    }
    case SymbolKind::Func: {
      if (const auto* def = db->GetFunc(sym).AnyDef()) {
        if (!def->hover.empty())
          return make(def->hover);
        if (!def->detailed_name.empty())
          return make(def->detailed_name);
      }
      return nullopt;
    }
    case SymbolKind::Var: {
      if (const auto* def = db->GetVar(sym).AnyDef()) {
        if (!def->hover.empty())
          return make(def->hover);
        if (!def->detailed_name.empty())
          return make(def->detailed_name);
      }
      return nullopt;
    }
    case SymbolKind::File:
    case SymbolKind::Invalid: {
      CQUERY_UNREACHABLE();
      break;
    }
  }
  CQUERY_UNREACHABLE();
  return nullopt;
}

struct In_TextDocumentHover : public RequestInMessage {
  MethodType GetMethodType() const override { return kMethodType; }
  lsTextDocumentPositionParams params;
};
MAKE_REFLECT_STRUCT(In_TextDocumentHover, id, params);
REGISTER_IN_MESSAGE(In_TextDocumentHover);

struct Out_TextDocumentHover : public lsOutMessage<Out_TextDocumentHover> {
  struct Result {
    std::vector<lsMarkedString> contents;
    optional<lsRange> range;
  };

  lsRequestId id;
  optional<Result> result;
};
MAKE_REFLECT_STRUCT(Out_TextDocumentHover::Result, contents, range);
void Reflect(Writer& visitor, Out_TextDocumentHover& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(jsonrpc);
  REFLECT_MEMBER(id);
  if (value.result)
    REFLECT_MEMBER(result);
  else {
    // Empty optional<> is elided by the default serializer, we need to write
    // |null| to be compliant with the LSP.
    visitor.Key("result");
    visitor.Null();
  }
  REFLECT_MEMBER_END();
}

struct Handler_TextDocumentHover : BaseMessageHandler<In_TextDocumentHover> {
  MethodType GetMethodType() const override { return kMethodType; }
  void Run(In_TextDocumentHover* request) override {
    QueryFile* file;
    if (!FindFileOrFail(db, project, request->id,
                        request->params.textDocument.uri.GetPath(), &file)) {
      return;
    }

    WorkingFile* working_file =
        working_files->GetFileByFilename(file->def->path);

    Out_TextDocumentHover out;
    out.id = request->id;

    for (SymbolRef sym :
         FindSymbolsAtLocation(working_file, file, request->params.position)) {
      // Found symbol. Return hover.
      optional<lsRange> ls_range = GetLsRange(
          working_files->GetFileByFilename(file->def->path), sym.range);
      if (!ls_range)
        continue;

      optional<lsMarkedString> comments = GetComments(db, sym);
      optional<lsMarkedString> hover =
          GetHoverOrName(db, file->def->language, sym);
      if (comments || hover) {
        out.result = Out_TextDocumentHover::Result();
        out.result->range = *ls_range;
        if (comments)
          out.result->contents.push_back(*comments);
        if (hover)
          out.result->contents.push_back(*hover);
        break;
      }
    }

    QueueManager::WriteStdout(kMethodType, out);
  }
};
REGISTER_MESSAGE_HANDLER(Handler_TextDocumentHover);
}  // namespace
