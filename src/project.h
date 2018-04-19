#pragma once

#include "config.h"
#include "method.h"

#include <optional.h>
#include <sparsepp/spp.h>

#include <functional>
#include <mutex>
#include <string>
#include <vector>

class QueueManager;
struct WorkingFiles;

struct Project {
  struct Entry {
    AbsolutePath filename;
    std::vector<std::string> args;
    // If true, this entry is inferred and was not read from disk.
    bool is_inferred = false;
  };

  // Include directories for "" headers
  std::vector<Directory> quote_include_directories;
  // Include directories for <> headers
  std::vector<Directory> angle_include_directories;

  std::vector<Entry> entries;
  spp::sparse_hash_map<AbsolutePath, int> absolute_path_to_entry_index_;

  // Loads a project for the given |directory|.
  //
  // If |g_config->compilationDatabaseDirectory| is not empty, look for .cquery
  // or compile_commands.json in it, otherwise they are retrieved in
  // |root_directory|.
  //
  // For .cquery, recursive directory listing is used and files with known
  // suffixes are indexed. .cquery files can exist in subdirectories and they
  // will affect flags in their subtrees (relative paths are relative to the
  // project root, not subdirectories). For compile_commands.json, its entries
  // are indexed.
  void Load(const AbsolutePath& root_directory);

  // Lookup the CompilationEntry for |filename|. If no entry was found this
  // will infer one based on existing project structure.
  Entry FindCompilationEntryForFile(const AbsolutePath& filename);

  // If the client has overridden the flags, or specified them for a file
  // that is not in the compilation_database.json make sure those changes
  // are permanent.
  void SetFlagsForFile(const std::vector<std::string>& flags,
                       const AbsolutePath& path);

  // Run |action| on every file in the project.
  void ForAllFilteredFiles(
      std::function<void(int i, const Entry& entry)> action);

  void Index(QueueManager* queue, WorkingFiles* working_files, lsRequestId id);
};
