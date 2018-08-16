#pragma once

#include "file_types.h"

#include <optional.h>
#include <string_view.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

// Trim from start (in place)
void TrimStartInPlace(std::string& s);
// Trim from end (in place)
void TrimEndInPlace(std::string& s);
// Trim from both ends (in place)
void TrimInPlace(std::string& s);
std::string Trim(std::string s);

uint64_t HashUsr(std::string_view s);

// Returns true if |value| starts/ends with |start| or |ending|.
bool StartsWith(std::string_view value, std::string_view start);
bool EndsWith(std::string_view value, std::string_view ending);
bool AnyStartsWith(const std::vector<std::string>& values,
                   const std::string& start);
bool StartsWithAny(const std::string& value,
                   const std::vector<std::string>& startings);
bool EndsWithAny(const std::string& value,
                 const std::vector<std::string>& endings);
bool FindAnyPartial(const std::string& value,
                    const std::vector<std::string>& values);
// Returns the dirname of |path|, i.e. "foo/bar.cc" => "foo/", "foo" => "./",
// "/foo" => "/". The result always ends in '/'.
std::string GetDirName(std::string path);
// Returns the basename of |path|, ie, "foo/bar.cc" => "bar.cc".
std::string GetBaseName(const std::string& path);
// Returns |path| without the filetype, ie, "foo/bar.cc" => "foo/bar".
std::string StripFileType(const std::string& path);

std::string ReplaceAll(const std::string& source,
                       const std::string& from,
                       const std::string& to);

std::vector<std::string> SplitString(const std::string& str,
                                     const std::string& delimiter);

template <typename TValues, typename TMap>
std::string StringJoinMap(const TValues& values,
                          const TMap& map,
                          const std::string& sep = ", ") {
  std::string result;
  bool first = true;
  for (auto& entry : values) {
    if (!first)
      result += sep;
    first = false;
    result += map(entry);
  }
  return result;
}

template <typename TValues>
std::string StringJoin(const TValues& values, const std::string& sep = ", ") {
  return StringJoinMap(values, [](const std::string& entry) { return entry; },
                       sep);
}

template <typename TCollection, typename TValue>
bool ContainsValue(const TCollection& collection, const TValue& value) {
  return std::find(std::begin(collection), std::end(collection), value) !=
         std::end(collection);
}

// Finds all files in the given folder. This is recursive.
std::vector<std::string> GetFilesAndDirectoriesInFolder(std::string folder,
                                          bool recursive,
                                          bool add_folder_to_path);
void GetFilesAndDirectoriesInFolder(std::string folder,
                      bool recursive,
                      bool add_folder_to_path,
                      const std::function<void(const std::string&)>& handler);

// Ensures that |path| ends in a slash.
void EnsureEndsInSlash(std::string& path);

// Converts a file path to one that can be used as filename.
// e.g. foo/bar.c => foo_bar.c
std::string EscapeFileName(std::string path);

// FIXME: Move ReadContent into ICacheManager?
bool FileExists(const std::string& filename);
optional<std::string> ReadContent(const AbsolutePath& filename);
std::vector<std::string> ReadLinesWithEnding(const AbsolutePath& filename);
std::vector<std::string> ToLines(const std::string& content,
                                 bool trim_whitespace);

struct TextReplacer {
  struct Replacement {
    std::string from;
    std::string to;
  };

  std::vector<Replacement> replacements;

  std::string Apply(const std::string& content);
};

void WriteToFile(const std::string& filename, const std::string& content);

template <typename T>
void AddRange(std::vector<T>* dest, const std::vector<T>& to_add) {
  dest->insert(dest->end(), to_add.begin(), to_add.end());
}

template <typename T>
void AddRange(std::vector<T>* dest, std::vector<T>&& to_add) {
  dest->insert(dest->end(), std::make_move_iterator(to_add.begin()),
               std::make_move_iterator(to_add.end()));
}

template <typename T, typename Fn>
void RemoveIf(std::vector<T>* vec, Fn predicate) {
  vec->erase(std::remove_if(vec->begin(), vec->end(), predicate), vec->end());
}

float GetProcessMemoryUsedInMb();

std::string FormatMicroseconds(long long microseconds);

optional<AbsolutePath> GetDefaultResourceDirectory();

// Makes sure all newlines in |output| are in \r\n format.
std::string UpdateToRnNewlines(std::string output);

// Get an executable located next to the cquery binary.
AbsolutePath GetExecutablePathNextToCqueryBinary(const std::string& name);

// Utility methods to check if |path| is absolute.
bool IsAbsolutePath(const std::string& path);
bool IsUnixAbsolutePath(const std::string& path);
bool IsWindowsAbsolutePath(const std::string& path);

bool IsDirectory(const std::string& path);

size_t HashArguments(const std::vector<std::string>& args);