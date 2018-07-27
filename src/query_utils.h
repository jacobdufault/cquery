#pragma once

#include "query.h"
#include "working_files.h"

#include <optional.h>

optional<QueryId::LexicalRef> GetDefinitionSpell(QueryDatabase* db,
                                                 SymbolIdx sym);
optional<QueryId::LexicalRef> GetDefinitionExtent(QueryDatabase* db,
                                                  SymbolIdx sym);
optional<QueryId::File> GetDeclarationFileForSymbol(QueryDatabase* db,
                                                    SymbolIdx sym);

// Get defining declaration (if exists) or an arbitrary declaration (otherwise)
// for each id.
std::vector<QueryId::LexicalRef> GetDeclarations(
    QueryDatabase* db,
    const std::vector<QueryId::Func>& ids);
std::vector<QueryId::LexicalRef> GetDeclarations(
    QueryDatabase* db,
    const std::vector<QueryId::Type>& ids);
std::vector<QueryId::LexicalRef> GetDeclarations(
    QueryDatabase* db,
    const std::vector<QueryId::Var>& ids);

// Get non-defining declarations.
std::vector<QueryId::LexicalRef> GetNonDefDeclarations(QueryDatabase* db,
                                                       SymbolIdx sym);

std::vector<QueryId::LexicalRef> GetRefsForAllBases(QueryDatabase* db,
                                                    QueryFunc& root);
std::vector<QueryId::LexicalRef> GetRefsForAllDerived(QueryDatabase* db,
                                                      QueryFunc& root);
optional<lsPosition> GetLsPosition(WorkingFile* working_file,
                                   const Position& position);
optional<lsRange> GetLsRange(WorkingFile* working_file, const Range& location);
lsDocumentUri GetLsDocumentUri(QueryDatabase* db,
                               QueryId::File file_id,
                               AbsolutePath* path);
lsDocumentUri GetLsDocumentUri(QueryDatabase* db, QueryId::File file_id);

optional<lsLocation> GetLsLocation(QueryDatabase* db,
                                   WorkingFiles* working_files,
                                   QueryId::LexicalRef ref);
std::vector<lsLocation> GetLsLocations(
    QueryDatabase* db,
    WorkingFiles* working_files,
    const std::vector<QueryId::LexicalRef>& refs);
// Returns a symbol. The symbol will have *NOT* have a location assigned.
optional<lsSymbolInformation> GetSymbolInfo(QueryDatabase* db,
                                            WorkingFiles* working_files,
                                            SymbolIdx sym,
                                            bool use_short_name);

std::vector<QueryId::SymbolRef> FindSymbolsAtLocation(WorkingFile* working_file,
                                                      QueryFile* file,
                                                      lsPosition position);

// Calls fn with a QueryFunc, QueryType, or QueryVar instance.
template <typename Fn>
void WithEntity(QueryDatabase* db, SymbolIdx sym, Fn&& fn) {
  switch (sym.kind) {
    case SymbolKind::Invalid:
    case SymbolKind::File:
      break;
    case SymbolKind::Func:
      fn(db->GetFunc(sym));
      break;
    case SymbolKind::Type:
      fn(db->GetType(sym));
      break;
    case SymbolKind::Var:
      fn(db->GetVar(sym));
      break;
  }
}

template <typename Fn>
void EachEntityDef(QueryDatabase* db, SymbolIdx sym, Fn&& fn) {
  WithEntity(db, sym, [&](const auto& entity) {
    for (auto& def : entity.def)
      if (!fn(def))
        break;
  });
}

template <typename Fn>
void EachOccurrence(QueryDatabase* db,
                    SymbolIdx sym,
                    bool include_decl,
                    Fn&& fn) {
  WithEntity(db, sym, [&](const auto& entity) {
    for (QueryId::LexicalRef ref : entity.uses)
      fn(ref);
    if (include_decl) {
      for (auto& def : entity.def)
        if (def.spell)
          fn(*def.spell);
      for (QueryId::LexicalRef ref : entity.declarations)
        fn(ref);
    }
  });
}

lsSymbolKind GetSymbolKind(QueryDatabase* db, SymbolIdx sym);

template <typename Fn>
void EachOccurrenceWithParent(QueryDatabase* db,
                              SymbolIdx sym,
                              bool include_decl,
                              Fn&& fn) {
  WithEntity(db, sym, [&](const auto& entity) {
    lsSymbolKind parent_kind = lsSymbolKind::Unknown;
    for (auto& def : entity.def)
      if (def.spell) {
        parent_kind = GetSymbolKind(db, sym);
        break;
      }
    for (QueryId::LexicalRef ref : entity.uses)
      fn(ref, parent_kind);
    if (include_decl) {
      for (auto& def : entity.def)
        if (def.spell)
          fn(*def.spell, parent_kind);
      for (QueryId::LexicalRef ref : entity.declarations)
        fn(ref, parent_kind);
    }
  });
}

template <typename Fn>
void EachDefinedType(QueryDatabase* db,
                     const std::vector<QueryId::Type>& ids,
                     Fn&& fn) {
  for (QueryId::Type id : ids) {
    QueryType& obj = db->GetType(id);
    if (!obj.def.empty())
      fn(obj);
  }
}
template <typename Fn>
void EachDefinedFunc(QueryDatabase* db,
                     const std::vector<QueryId::Func>& ids,
                     Fn&& fn) {
  for (QueryId::Func id : ids) {
    QueryFunc& obj = db->GetFunc(id);
    if (!obj.def.empty())
      fn(obj);
  }
}
template <typename Fn>
void EachDefinedVar(QueryDatabase* db,
                    const std::vector<QueryId::Var>& ids,
                    Fn&& fn) {
  for (QueryId::Var id : ids) {
    QueryVar& obj = db->GetVar(id);
    if (!obj.def.empty())
      fn(obj);
  }
}