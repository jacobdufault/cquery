#pragma once

#include "file_types.h"

#include <optional.h>
#include <string_view.h>

#include <memory>
#include <string>
#include <vector>

struct PlatformMutex {
  virtual ~PlatformMutex();
};
struct PlatformScopedMutexLock {
  virtual ~PlatformScopedMutexLock();
};
struct PlatformSharedMemory {
  virtual ~PlatformSharedMemory();
  void* data;
  size_t capacity;
  std::string name;
};

void PlatformInit();

AbsolutePath GetExecutablePath();
AbsolutePath GetWorkingDirectory();
optional<AbsolutePath> NormalizePath(const std::string& path,
                                     bool ensure_exists = true,
                                     bool force_lower_on_windows = true);

// Creates a directory at |path|. Creates directories recursively if needed.
void MakeDirectoryRecursive(const AbsolutePath& path);
// Tries to create the directory given by |absolute_path|. Returns true if
// successful or if the directory already exists. Returns false otherwise. This
// does not attempt to recursively create directories.
bool TryMakeDirectory(const AbsolutePath& path);

// tmpl: template string for mkdtemp(3), ignored on windows.
optional<AbsolutePath> TryMakeTempDirectory(char *tmpl);

void SetCurrentThreadName(const std::string& thread_name);

optional<int64_t> GetLastModificationTime(const AbsolutePath& absolute_path);

void MoveFileTo(const AbsolutePath& destination, const AbsolutePath& source);
void CopyFileTo(const AbsolutePath& destination, const AbsolutePath& source);

bool IsSymLink(const AbsolutePath& path);

// Returns any clang arguments that are specific to the current platform.
std::vector<const char*> GetPlatformClangArguments();

// Free any unused memory and return it to the system.
void FreeUnusedMemory();

// If true objective-c index tests will be run.
bool RunObjectiveCIndexTests();

// Stop self and wait for SIGCONT.
void TraceMe();

optional<std::string> RunExecutable(const std::vector<std::string>& command,
                                    std::string_view input);

optional<std::string> GetGlobalConfigDirectory();
