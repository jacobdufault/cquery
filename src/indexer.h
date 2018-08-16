#pragma once

#include "clang_cursor.h"
#include "clang_index.h"
#include "clang_translation_unit.h"
#include "clang_utils.h"
#include "file_consumer.h"
#include "file_contents.h"
#include "language.h"
#include "lsp.h"
#include "maybe.h"
#include "position.h"
#include "project.h"
#include "serializer.h"
#include "symbol.h"
#include "utils.h"

#include <optional.h>
#include <string_view.h>

#include <ctype.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <vector>

struct IndexFile;
struct IndexType;
struct IndexFunc;
struct IndexVar;
struct QueryFile;

using RawId = uint32_t;

template <typename T>
struct Id {
  RawId id;

  // Invalid id.
  Id() : id(-1) {}
  explicit Id(RawId id) : id(id) {}
  // Id<T> -> Id<void> or Id<T> -> Id<T> is allowed implicitly.
  template <typename U,
            typename std::enable_if<std::is_void<T>::value ||
                                        std::is_same<T, U>::value,
                                    bool>::type = false>
  Id(Id<U> o) : id(o.id) {}
  template <typename U,
            typename std::enable_if<!(std::is_void<T>::value ||
                                      std::is_same<T, U>::value),
                                    bool>::type = false>
  explicit Id(Id<U> o) : id(o.id) {}

  // Needed for google::dense_hash_map.
  explicit operator RawId() const { return id; }

  bool HasValueForMaybe_() const { return id != RawId(-1); }

  bool operator==(const Id& o) const { return id == o.id; }
  bool operator!=(const Id& o) const { return id != o.id; }
  bool operator<(const Id& o) const { return id < o.id; }
};
using AnyId = Id<void>;

namespace std {
template <typename T>
struct hash<Id<T>> {
  size_t operator()(const Id<T>& k) const { return hash<RawId>()(k.id); }
};
}  // namespace std

template <typename TVisitor, typename T>
void Reflect(TVisitor& visitor, Id<T>& id) {
  Reflect(visitor, id.id);
}

struct SymbolIdx {
  AnyId id;
  SymbolKind kind;

  bool operator==(const SymbolIdx& o) const {
    return id == o.id && kind == o.kind;
  }
  bool operator!=(const SymbolIdx& o) const { return !(*this == o); }
  bool operator<(const SymbolIdx& o) const {
    if (id != o.id)
      return id < o.id;
    return kind < o.kind;
  }
};
MAKE_REFLECT_STRUCT(SymbolIdx, kind, id);
MAKE_HASHABLE(SymbolIdx, t.kind, t.id);

// The meaning of |id|, |kind| are determined if this is a SymbolRef or a
// LexicalRef. This type should not be constructed directly.
struct Reference {
  Range range;
  AnyId id;
  SymbolKind kind;
  Role role;

  bool HasValueForMaybe_() const { return range.HasValueForMaybe_(); }
  operator SymbolIdx() const { return {id, kind}; }
  std::tuple<Range, AnyId, SymbolKind, Role> ToTuple() const {
    return std::make_tuple(range, id, kind, role);
  }
  bool operator==(const Reference& o) const { return ToTuple() == o.ToTuple(); }
  bool operator<(const Reference& o) const { return ToTuple() < o.ToTuple(); }
};

// |id|,|kind| refer to the referenced entity.
struct IndexSymbolRef : Reference {
  IndexSymbolRef() = default;
  IndexSymbolRef(Range range, AnyId id, SymbolKind kind, Role role)
      : Reference{range, id, kind, role} {}
};

// |id|,|kind| refer to the lexical parent.
struct IndexLexicalRef : Reference {
  IndexLexicalRef() = default;
  IndexLexicalRef(Range range, AnyId id, SymbolKind kind, Role role)
      : Reference{range, id, kind, role} {}
};

struct IndexId {
  using File = Id<IndexFile>;
  using Func = Id<IndexFunc>;
  using Type = Id<IndexType>;
  using Var = Id<IndexVar>;
  using SymbolRef = IndexSymbolRef;
  using LexicalRef = IndexLexicalRef;
};

void Reflect(Reader& visitor, Reference& value);
void Reflect(Writer& visitor, Reference& value);

template <typename Id>
struct TypeDefDefinitionData {
  // General metadata.
  std::string detailed_name;
  std::string hover;
  std::string comments;

  // While a class/type can technically have a separate declaration/definition,
  // it doesn't really happen in practice. The declaration never contains
  // comments or insightful information. The user always wants to jump from
  // the declaration to the definition - never the other way around like in
  // functions and (less often) variables.
  //
  // It's also difficult to identify a `class Foo;` statement with the clang
  // indexer API (it's doable using cursor AST traversal), so we don't bother
  // supporting the feature.
  Maybe<typename Id::LexicalRef> spell;
  Maybe<typename Id::LexicalRef> extent;

  // Immediate parent types.
  std::vector<typename Id::Type> bases;

  // Types, functions, and variables defined in this type.
  std::vector<typename Id::Type> types;
  std::vector<typename Id::Func> funcs;
  std::vector<typename Id::Var> vars;

  typename Id::File file;
  // If set, then this is the same underlying type as the given value (ie, this
  // type comes from a using or typedef statement).
  Maybe<typename Id::Type> alias_of;

  int16_t short_name_offset = 0;
  int16_t short_name_size = 0;
  lsSymbolKind kind = lsSymbolKind::Unknown;

  bool operator==(const TypeDefDefinitionData& o) const {
    return detailed_name == o.detailed_name && spell == o.spell &&
           extent == o.extent && alias_of == o.alias_of && bases == o.bases &&
           types == o.types && funcs == o.funcs && vars == o.vars &&
           kind == o.kind && hover == o.hover && comments == o.comments;
  }
  bool operator!=(const TypeDefDefinitionData& o) const {
    return !(*this == o);
  }

  std::string_view ShortName() const {
    return std::string_view(detailed_name.c_str() + short_name_offset,
                            short_name_size);
  }
  // Used by cquery_inheritance_hierarchy.cc:Expand generic lambda
  std::string_view DetailedName(bool) const { return detailed_name; }
};
template <typename TVisitor, typename Family>
void Reflect(TVisitor& visitor, TypeDefDefinitionData<Family>& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(detailed_name);
  REFLECT_MEMBER(short_name_offset);
  REFLECT_MEMBER(short_name_size);
  REFLECT_MEMBER(kind);
  REFLECT_MEMBER(hover);
  REFLECT_MEMBER(comments);
  REFLECT_MEMBER(spell);
  REFLECT_MEMBER(extent);
  REFLECT_MEMBER(file);
  REFLECT_MEMBER(alias_of);
  REFLECT_MEMBER(bases);
  REFLECT_MEMBER(types);
  REFLECT_MEMBER(funcs);
  REFLECT_MEMBER(vars);
  REFLECT_MEMBER_END();
}

struct IndexType {
  using Def = TypeDefDefinitionData<IndexId>;

  Usr usr;
  IndexId::Type id;

  Def def;
  std::vector<IndexId::LexicalRef> declarations;

  // Immediate derived types.
  std::vector<IndexId::Type> derived;

  // Declared variables of this type.
  std::vector<IndexId::Var> instances;

  // Every usage, useful for things like renames.
  // NOTE: Do not insert directly! Use AddUsage instead.
  std::vector<IndexId::LexicalRef> uses;

  IndexType() {}  // For serialization.
  IndexType(IndexId::Type id, Usr usr);

  bool operator<(const IndexType& other) const { return id < other.id; }
};
MAKE_HASHABLE(IndexType, t.id);

template <typename Id>
struct FuncDefDefinitionData {
  // General metadata.
  std::string detailed_name;
  std::string hover;
  std::string comments;
  Maybe<typename Id::LexicalRef> spell;
  Maybe<typename Id::LexicalRef> extent;

  // Method this method overrides.
  std::vector<typename Id::Func> bases;

  // Local variables or parameters.
  std::vector<typename Id::Var> vars;

  // Functions that this function calls.
  std::vector<typename Id::SymbolRef> callees;

  typename Id::File file;
  // Type which declares this one (ie, it is a method)
  Maybe<typename Id::Type> declaring_type;
  int16_t short_name_offset = 0;
  int16_t short_name_size = 0;
  lsSymbolKind kind = lsSymbolKind::Unknown;
  StorageClass storage = StorageClass::Invalid;

  bool operator==(const FuncDefDefinitionData& o) const {
    return detailed_name == o.detailed_name && spell == o.spell &&
           extent == o.extent && declaring_type == o.declaring_type &&
           bases == o.bases && vars == o.vars && callees == o.callees &&
           kind == o.kind && storage == o.storage && hover == o.hover &&
           comments == o.comments;
  }
  bool operator!=(const FuncDefDefinitionData& o) const {
    return !(*this == o);
  }

  std::string_view ShortName() const {
    return std::string_view(detailed_name.c_str() + short_name_offset,
                            short_name_size);
  }
  std::string_view DetailedName(bool params) const {
    if (params)
      return detailed_name;
    return std::string_view(detailed_name)
        .substr(0, short_name_offset + short_name_size);
  }
};

template <typename TVisitor, typename Family>
void Reflect(TVisitor& visitor, FuncDefDefinitionData<Family>& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(detailed_name);
  REFLECT_MEMBER(short_name_offset);
  REFLECT_MEMBER(short_name_size);
  REFLECT_MEMBER(kind);
  REFLECT_MEMBER(storage);
  REFLECT_MEMBER(hover);
  REFLECT_MEMBER(comments);
  REFLECT_MEMBER(spell);
  REFLECT_MEMBER(extent);
  REFLECT_MEMBER(file);
  REFLECT_MEMBER(declaring_type);
  REFLECT_MEMBER(bases);
  REFLECT_MEMBER(vars);
  REFLECT_MEMBER(callees);
  REFLECT_MEMBER_END();
}

struct IndexFunc {
  using Def = FuncDefDefinitionData<IndexId>;

  Usr usr;
  IndexId::Func id;

  Def def;

  struct Declaration {
    // Range of only the function name.
    IndexId::LexicalRef spell;
    // Location of the parameter names.
    std::vector<Range> param_spellings;
  };

  // Places the function is forward-declared.
  std::vector<Declaration> declarations;

  // Methods which directly override this one.
  std::vector<IndexId::Func> derived;

  // Calls/usages of this function. If the call is coming from outside a
  // function context then the FuncRef will not have an associated id.
  //
  // To get all usages, also include the ranges inside of declarations and
  // def.spell.
  std::vector<IndexId::LexicalRef> uses;

  IndexFunc() {}  // For serialization.
  IndexFunc(IndexId::Func id, Usr usr) : usr(usr), id(id) {}

  bool operator<(const IndexFunc& other) const { return id < other.id; }
};
MAKE_HASHABLE(IndexFunc, t.id);
MAKE_REFLECT_STRUCT(IndexFunc::Declaration, spell, param_spellings);

template <typename Id>
struct VarDefDefinitionData {
  // General metadata.
  std::string detailed_name;
  std::string hover;
  std::string comments;
  // TODO: definitions should be a list of ranges, since there can be more
  //       than one - when??
  Maybe<typename Id::LexicalRef> spell;
  Maybe<typename Id::LexicalRef> extent;

  typename Id::File file;
  // Type of the variable.
  Maybe<typename Id::Type> type;

  // Function/type which declares this one.
  int16_t short_name_offset = 0;
  int16_t short_name_size = 0;

  lsSymbolKind kind = lsSymbolKind::Unknown;
  // Note a variable may have instances of both |None| and |Extern|
  // (declaration).
  StorageClass storage = StorageClass::Invalid;

  bool is_local() const { return kind == lsSymbolKind::Variable; }

  bool operator==(const VarDefDefinitionData& o) const {
    return detailed_name == o.detailed_name && spell == o.spell &&
           extent == o.extent && type == o.type && kind == o.kind &&
           storage == o.storage && hover == o.hover && comments == o.comments;
  }
  bool operator!=(const VarDefDefinitionData& o) const { return !(*this == o); }

  std::string_view ShortName() const {
    return std::string_view(detailed_name.c_str() + short_name_offset,
                            short_name_size);
  }
  std::string DetailedName(bool qualified) const {
    if (qualified)
      return detailed_name;
    int i = short_name_offset;
    for (int paren = 0; i; i--) {
      // Skip parentheses in "(anon struct)::name"
      if (detailed_name[i - 1] == ')')
        paren++;
      else if (detailed_name[i - 1] == '(')
        paren--;
      else if (!(paren > 0 || isalnum(detailed_name[i - 1]) ||
                 detailed_name[i - 1] == '_' || detailed_name[i - 1] == ':'))
        break;
    }
    return detailed_name.substr(0, i) + detailed_name.substr(short_name_offset);
  }
};

template <typename TVisitor, typename Family>
void Reflect(TVisitor& visitor, VarDefDefinitionData<Family>& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(detailed_name);
  REFLECT_MEMBER(short_name_size);
  REFLECT_MEMBER(short_name_offset);
  REFLECT_MEMBER(hover);
  REFLECT_MEMBER(comments);
  REFLECT_MEMBER(spell);
  REFLECT_MEMBER(extent);
  REFLECT_MEMBER(file);
  REFLECT_MEMBER(type);
  REFLECT_MEMBER(kind);
  REFLECT_MEMBER(storage);
  REFLECT_MEMBER_END();
}

struct IndexVar {
  using Def = VarDefDefinitionData<IndexId>;

  Usr usr;
  IndexId::Var id;

  Def def;

  std::vector<IndexId::LexicalRef> declarations;
  std::vector<IndexId::LexicalRef> uses;

  IndexVar() {}  // For serialization.
  IndexVar(IndexId::Var id, Usr usr) : usr(usr), id(id) {}

  bool operator<(const IndexVar& other) const { return id < other.id; }
};
MAKE_HASHABLE(IndexVar, t.id);

struct IdCache {
  AbsolutePath primary_file;
  std::unordered_map<Usr, IndexId::Type> usr_to_type_id;
  std::unordered_map<Usr, IndexId::Func> usr_to_func_id;
  std::unordered_map<Usr, IndexId::Var> usr_to_var_id;
  std::unordered_map<IndexId::Type, Usr> type_id_to_usr;
  std::unordered_map<IndexId::Func, Usr> func_id_to_usr;
  std::unordered_map<IndexId::Var, Usr> var_id_to_usr;

  IdCache(const AbsolutePath& primary_file);
};

struct IndexInclude {
  // Line that has the include directive. We don't have complete range
  // information - a line is good enough for clicking.
  int line = 0;
  // Absolute path to the index.
  std::string resolved_path;
};

struct IndexFile {
  IdCache id_cache;

  // For both JSON and MessagePack cache files.
  static const int kMajorVersion;
  // For MessagePack cache files.
  // JSON has good forward compatibility because field addition/deletion do not
  // harm but currently no efforts have been made to make old MessagePack cache
  // files accepted by newer cquery.
  static const int kMinorVersion;

  AbsolutePath path;
  size_t args_hash;
  int64_t last_modification_time = 0;
  LanguageId language = LanguageId::Unknown;

  // The path to the translation unit cc file which caused the creation of this
  // IndexFile. When parsing a translation unit we generate many IndexFile
  // instances (ie, each header has a separate one). When the user edits a
  // header we need to lookup the original translation unit and reindex that.
  AbsolutePath import_file;

  // Source ranges that were not processed.
  std::vector<Range> skipped_by_preprocessor;

  std::vector<IndexInclude> includes;
  std::vector<AbsolutePath> dependencies;
  std::vector<IndexType> types;
  std::vector<IndexFunc> funcs;
  std::vector<IndexVar> vars;

  // Diagnostics found when indexing this file. Not serialized.
  std::vector<lsDiagnostic> diagnostics_;
  // File contents at the time of index. Not serialized.
  std::string file_contents;

  IndexFile(const AbsolutePath& path);

  IndexId::Type ToTypeId(Usr usr);
  IndexId::Func ToFuncId(Usr usr);
  IndexId::Var ToVarId(Usr usr);
  IndexId::Type ToTypeId(const CXCursor& usr);
  IndexId::Func ToFuncId(const CXCursor& usr);
  IndexId::Var ToVarId(const CXCursor& usr);
  IndexType* Resolve(IndexId::Type id);
  IndexFunc* Resolve(IndexId::Func id);
  IndexVar* Resolve(IndexId::Var id);

  std::string ToString();
};

struct NamespaceHelper {
  std::unordered_map<ClangCursor, std::string>
      container_cursor_to_qualified_name;

  std::string QualifiedName(const CXIdxContainerInfo* container,
                            std::string_view unqualified_name);
};

// |import_file| is the cc file which is what gets passed to clang.
// |desired_index_file| is the (h or cc) file which has actually changed.
// |dependencies| are the existing dependencies of |import_file| if this is a
// reparse.
optional<std::vector<std::unique_ptr<IndexFile>>> Parse(
    FileConsumerSharedState* file_consumer_shared,
    const std::string& file,
    const std::vector<std::string>& args,
    const std::vector<FileContents>& file_contents,
    ClangIndex* index,
    bool dump_ast = false);

void ConcatTypeAndName(std::string& type, const std::string& name);

void IndexInit();

void ClangSanityCheck(const Project::Entry& entry);

std::string GetClangVersion();
