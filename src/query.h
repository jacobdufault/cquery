#pragma once

#include "indexer.h"
#include "serializer.h"

#include <sparsepp/spp.h>

#include <functional>

struct QueryFile;
struct QueryType;
struct QueryFunc;
struct QueryVar;
struct QueryDatabase;

struct IdMap;

// |id|,|kind| refer to the referenced entity.
struct QuerySymbolRef : Reference {
  QuerySymbolRef() = default;
  QuerySymbolRef(Range range, AnyId id, SymbolKind kind, Role role)
      : Reference{range, id, kind, role} {}
};

// |id|,|kind| refer to the lexical parent.
struct QueryLexicalRef : Reference {
  Id<QueryFile> file;
  QueryLexicalRef() = default;
  QueryLexicalRef(Range range,
                  AnyId id,
                  SymbolKind kind,
                  Role role,
                  Id<QueryFile> file)
      : Reference{range, id, kind, role}, file(file) {}
};
// Used by |HANDLE_MERGEABLE| so only |range| is needed.
MAKE_HASHABLE(QueryLexicalRef, t.range);

struct QueryId {
  using File = Id<QueryFile>;
  using Func = Id<QueryFunc>;
  using Type = Id<QueryType>;
  using Var = Id<QueryVar>;
  using SymbolRef = QuerySymbolRef;
  using LexicalRef = QueryLexicalRef;
};

// There are two sources of reindex updates: the (single) definition of a
// symbol has changed, or one of many users of the symbol has changed.
//
// For simplicitly, if the single definition has changed, we update all of the
// associated single-owner definition data. See |Update*DefId|.
//
// If one of the many symbol users submits an update, we store the update such
// that it can be merged with other updates before actually being applied to
// the main database. See |MergeableUpdate|.

template <typename TId, typename TValue>
struct MergeableUpdate {
  // The type/func/var which is getting new usages.
  TId id;
  // Entries to add and remove.
  std::vector<TValue> to_add;
  std::vector<TValue> to_remove;

  MergeableUpdate(TId id, std::vector<TValue>&& to_add)
      : id(id), to_add(std::move(to_add)) {}
  MergeableUpdate(TId id,
                  std::vector<TValue>&& to_add,
                  std::vector<TValue>&& to_remove)
      : id(id), to_add(std::move(to_add)), to_remove(std::move(to_remove)) {}
};
template <typename TVisitor, typename TId, typename TValue>
void Reflect(TVisitor& visitor, MergeableUpdate<TId, TValue>& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(id);
  REFLECT_MEMBER(to_add);
  REFLECT_MEMBER(to_remove);
  REFLECT_MEMBER_END();
}

template <typename TId, typename TValue>
struct WithId {
  TId id;
  TValue value;

  WithId(TId id, const TValue& value) : id(id), value(value) {}
  WithId(TId id, TValue&& value) : id(id), value(std::move(value)) {}
};
template <typename TVisitor, typename TId, typename TValue>
void Reflect(TVisitor& visitor, WithId<TId, TValue>& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(id);
  REFLECT_MEMBER(value);
  REFLECT_MEMBER_END();
}

struct QueryFile {
  struct Def {
    QueryId::File file;
    AbsolutePath path;
    size_t args_hash;
    // Language identifier
    std::string language;
    // Includes in the file.
    std::vector<IndexInclude> includes;
    // Outline of the file (ie, for code lens).
    std::vector<QueryId::SymbolRef> outline;
    // Every symbol found in the file (ie, for goto definition)
    std::vector<QueryId::SymbolRef> all_symbols;
    // Parts of the file which are disabled.
    std::vector<Range> inactive_regions;
    // Used by |$cquery/freshenIndex|.
    std::vector<AbsolutePath> dependencies;
  };

  struct DefUpdate {
    QueryId::File id;
    std::string file_content;
    Def value;
  };
  optional<Def> def;
  size_t symbol_idx = -1;

  explicit QueryFile(const AbsolutePath& path) {
    def = Def();
    def->path = path;
  }
};
MAKE_REFLECT_STRUCT(QueryFile::Def,
                    file,
                    path,
                    args_hash,
                    language,
                    outline,
                    all_symbols,
                    inactive_regions,
                    dependencies);

template <typename TDerived, typename TDefinitionData>
struct QueryEntity {
  using Def = TDefinitionData;
  using DefUpdate = WithId<Id<TDerived>, Def>;
  using DeclarationsUpdate = MergeableUpdate<Id<TDerived>, QueryId::LexicalRef>;
  using UsesUpdate = MergeableUpdate<Id<TDerived>, QueryId::LexicalRef>;

  Def* AnyDef() {
    Def* ret = nullptr;
    for (auto& i : static_cast<TDerived*>(this)->def) {
      ret = &i;
      if (i.spell)
        break;
    }
    return ret;
  }
  const Def* AnyDef() const { return const_cast<QueryEntity*>(this)->AnyDef(); }
};

struct QueryType : QueryEntity<QueryType, TypeDefDefinitionData<QueryId>> {
  using DerivedUpdate = MergeableUpdate<QueryId::Type, QueryId::Type>;
  using InstancesUpdate = MergeableUpdate<QueryId::Type, QueryId::Var>;

  Usr usr;
  size_t symbol_idx = -1;
  std::vector<Def> def;
  std::vector<QueryId::LexicalRef> declarations;
  std::vector<QueryId::Type> derived;
  std::vector<QueryId::Var> instances;
  std::vector<QueryId::LexicalRef> uses;

  explicit QueryType(const Usr& usr) : usr(usr) {}
};

struct QueryFunc : QueryEntity<QueryFunc, FuncDefDefinitionData<QueryId>> {
  using DerivedUpdate = MergeableUpdate<QueryId::Func, QueryId::Func>;

  Usr usr;
  size_t symbol_idx = -1;
  std::vector<Def> def;
  std::vector<QueryId::LexicalRef> declarations;
  std::vector<QueryId::Func> derived;
  std::vector<QueryId::LexicalRef> uses;

  explicit QueryFunc(const Usr& usr) : usr(usr) {}
};

struct QueryVar : QueryEntity<QueryVar, VarDefDefinitionData<QueryId>> {
  Usr usr;
  size_t symbol_idx = -1;
  std::vector<Def> def;
  std::vector<QueryId::LexicalRef> declarations;
  std::vector<QueryId::LexicalRef> uses;

  explicit QueryVar(const Usr& usr) : usr(usr) {}
};

struct IndexUpdate {
  // Creates a new IndexUpdate based on the delta from previous to current. If
  // no delta computation should be done just pass null for previous.
  static IndexUpdate CreateDelta(const IdMap* previous_id_map,
                                 const IdMap* current_id_map,
                                 IndexFile* previous,
                                 IndexFile* current);

  // Merge |update| into this update; this can reduce overhead / index update
  // work can be parallelized.
  void Merge(IndexUpdate&& update);

  // File updates.
  std::vector<AbsolutePath> files_removed;
  std::vector<QueryFile::DefUpdate> files_def_update;

  // Type updates.
  std::vector<WithId<QueryId::File, QueryId::Type>> types_removed;
  std::vector<QueryType::DefUpdate> types_def_update;
  std::vector<QueryType::DeclarationsUpdate> types_declarations;
  std::vector<QueryType::DerivedUpdate> types_derived;
  std::vector<QueryType::InstancesUpdate> types_instances;
  std::vector<QueryType::UsesUpdate> types_uses;

  // Function updates.
  std::vector<WithId<QueryId::File, QueryId::Func>> funcs_removed;
  std::vector<QueryFunc::DefUpdate> funcs_def_update;
  std::vector<QueryFunc::DeclarationsUpdate> funcs_declarations;
  std::vector<QueryFunc::DerivedUpdate> funcs_derived;
  std::vector<QueryFunc::UsesUpdate> funcs_uses;

  // Variable updates.
  std::vector<WithId<QueryId::File, QueryId::Var>> vars_removed;
  std::vector<QueryVar::DefUpdate> vars_def_update;
  std::vector<QueryVar::DeclarationsUpdate> vars_declarations;
  std::vector<QueryVar::UsesUpdate> vars_uses;

 private:
  // Creates an index update assuming that |previous| is already
  // in the index, so only the delta between |previous| and |current|
  // will be applied.
  IndexUpdate(const IdMap& previous_id_map,
              const IdMap& current_id_map,
              IndexFile& previous,
              IndexFile& current);
};

// The query database is heavily optimized for fast queries. It is stored
// in-memory.
struct QueryDatabase {
  // All File/Func/Type/Var symbols.
  std::vector<SymbolIdx> symbols;

  // Raw data storage. Accessible via SymbolIdx instances.
  std::vector<QueryFile> files;
  std::vector<QueryType> types;
  std::vector<QueryFunc> funcs;
  std::vector<QueryVar> vars;

  // Lookup symbol based on a usr.
  spp::sparse_hash_map<AbsolutePath, QueryId::File> usr_to_file;
  spp::sparse_hash_map<Usr, QueryId::Type> usr_to_type;
  spp::sparse_hash_map<Usr, QueryId::Func> usr_to_func;
  spp::sparse_hash_map<Usr, QueryId::Var> usr_to_var;

  // Removes data for the given ids in the given files.
  void Remove(const std::vector<WithId<QueryId::File, QueryId::Type>>& to_remove);
  void Remove(const std::vector<WithId<QueryId::File, QueryId::Func>>& to_remove);
  void Remove(const std::vector<WithId<QueryId::File, QueryId::Var>>& to_remove);

  // Insert the contents of |update| into |db|.
  void ApplyIndexUpdate(IndexUpdate* update);
  void ImportOrUpdate(const std::vector<QueryFile::DefUpdate>& updates);
  void ImportOrUpdate(std::vector<QueryType::DefUpdate>&& updates);
  void ImportOrUpdate(std::vector<QueryFunc::DefUpdate>&& updates);
  void ImportOrUpdate(std::vector<QueryVar::DefUpdate>&& updates);
  void UpdateSymbols(size_t* symbol_idx, SymbolKind kind, AnyId idx);
  std::string_view GetSymbolDetailedName(RawId symbol_idx) const;
  std::string_view GetSymbolShortName(RawId symbol_idx) const;

  QueryFile& GetFile(QueryId::File id);
  QueryFunc& GetFunc(QueryId::Func id);
  QueryType& GetType(QueryId::Type id);
  QueryVar& GetVar(QueryId::Var id);

  QueryFile& GetFile(SymbolIdx id);
  QueryFunc& GetFunc(SymbolIdx id);
  QueryType& GetType(SymbolIdx id);
  QueryVar& GetVar(SymbolIdx id);
};

template <typename I>
struct IndexToQuery;

// clang-format off
template <> struct IndexToQuery<IndexId::File> { using type = QueryId::File; };
template <> struct IndexToQuery<IndexId::Func> { using type = QueryId::Func; };
template <> struct IndexToQuery<IndexId::Type> { using type = QueryId::Type; };
template <> struct IndexToQuery<IndexId::Var> { using type = QueryId::Var; };
template <> struct IndexToQuery<IndexId::SymbolRef> { using type = QueryId::SymbolRef; };
template <> struct IndexToQuery<IndexId::LexicalRef> { using type = QueryId::LexicalRef; };
template <> struct IndexToQuery<IndexFunc::Declaration> { using type = QueryId::LexicalRef; };
template <typename I> struct IndexToQuery<optional<I>> {
  using type = optional<typename IndexToQuery<I>::type>;
};
template <typename I> struct IndexToQuery<std::vector<I>> {
  using type = std::vector<typename IndexToQuery<I>::type>;
};
// clang-format on

struct IdMap {
  const IdCache& local_ids;
  QueryId::File primary_file;

  IdMap(QueryDatabase* query_db, const IdCache& local_ids);

  // clang-format off
  Id<void> ToQuery(SymbolKind kind, Id<void> id) const;
  QueryId::Type ToQuery(IndexId::Type id) const;
  QueryId::Func ToQuery(IndexId::Func id) const;
  QueryId::Var ToQuery(IndexId::Var id) const;
  QueryId::SymbolRef ToQuery(IndexId::SymbolRef ref) const;
  QueryId::LexicalRef ToQuery(IndexId::LexicalRef ref) const;
  QueryId::LexicalRef ToQuery(IndexFunc::Declaration decl) const;
  template <typename I>
  Maybe<typename IndexToQuery<I>::type> ToQuery(Maybe<I> id) const {
    if (!id)
      return nullopt;
    return ToQuery(*id);
  }
  template <typename I>
  std::vector<typename IndexToQuery<I>::type> ToQuery(const std::vector<I>& a) const {
    std::vector<typename IndexToQuery<I>::type> ret;
    ret.reserve(a.size());
    for (auto& x : a)
      ret.push_back(ToQuery(x));
    return ret;
  }
  // clang-format on

 private:
  spp::sparse_hash_map<IndexId::Type, QueryId::Type> cached_type_ids_;
  spp::sparse_hash_map<IndexId::Func, QueryId::Func> cached_func_ids_;
  spp::sparse_hash_map<IndexId::Var, QueryId::Var> cached_var_ids_;
};
