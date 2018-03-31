#if defined(_WIN32)
#include "platform.h"

#include "utils.h"

#include <loguru.hpp>

#include <shlwapi.h>
#include <Windows.h>
#include <direct.h>
#include <fcntl.h>
#include <io.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

#pragma comment(lib, "Shlwapi.lib")

void PlatformInit() {
  // We need to write to stdout in binary mode because in Windows, writing
  // \n will implicitly write \r\n. Language server API will ignore a
  // \r\r\n split request.
  _setmode(_fileno(stdout), O_BINARY);
  _setmode(_fileno(stdin), O_BINARY);
}

// See
// https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
AbsolutePath GetExecutablePath() {
  char result[MAX_PATH] = {0};
  GetModuleFileName(NULL, result, MAX_PATH);
  return *NormalizePath(result);
}

// See http://stackoverflow.com/a/19535628
AbsolutePath GetWorkingDirectory() {
  char result[MAX_PATH];
  std::string binary_path(result, GetModuleFileName(NULL, result, MAX_PATH));
  return *NormalizePath(GetDirName(binary_path));
}

optional<AbsolutePath> NormalizePath(const std::string& path0) {
  // Requires Windows 8
  /*
  if (!PathCanonicalize(buffer, path.c_str()))
    return nullopt;
  */

  // Returns the volume name, ie, c:/
  /*
  if (!GetVolumePathName(path.c_str(), buffer, MAX_PATH))
    return nullopt;
  */

  std::string path = path0;

  TCHAR buffer[MAX_PATH] = TEXT("");

  // Normalize the path name, ie, resolve `..`.
  DWORD len = GetFullPathName(path.c_str(), MAX_PATH, buffer, nullptr);
  if (!len)
    return nullopt;
  path = std::string(buffer, len);

  // Get the actual casing of the path, ie, if the file on disk is `C:\FooBar`
  // and this function is called with `c:\fooBar` this will return `c:\FooBar`.
  // (drive casing is lowercase).
  len = GetLongPathName(path.c_str(), buffer, MAX_PATH);
  if (!len)
    return nullopt;
  path = std::string(buffer, len);

  // Empty paths have no meaning.
  if (path.empty())
    return nullopt;

  // We may need to normalize the drive name to upper-case; at the moment
  // vscode sends lower-case path names.
  /*
  path[0] = toupper(path[0]);
  */

  // cquery assumes forward-slashes.
  std::replace(path.begin(), path.end(), '\\', '/');

  LOG_S(ERROR) << path;

  return path;
}

bool TryMakeDirectory(const AbsolutePath& absolute_path) {
  if (_mkdir(absolute_path.path.c_str()) == -1) {
    // Success if the directory exists.
    return errno == EEXIST;
  }
  return true;
}

// See https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
  DWORD dwType;      // Must be 0x1000.
  LPCSTR szName;     // Pointer to name (in user addr space).
  DWORD dwThreadID;  // Thread ID (-1=caller thread).
  DWORD dwFlags;     // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)
void SetCurrentThreadName(const std::string& thread_name) {
  loguru::set_thread_name(thread_name.c_str());

  THREADNAME_INFO info;
  info.dwType = 0x1000;
  info.szName = thread_name.c_str();
  info.dwThreadID = (DWORD)-1;
  info.dwFlags = 0;

  __try {
    RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR),
                   (ULONG_PTR*)&info);
#ifdef _MSC_VER
  } __except (EXCEPTION_EXECUTE_HANDLER) {
#else
  } catch (...) {
#endif
  }
}

optional<int64_t> GetLastModificationTime(const AbsolutePath& absolute_path) {
  struct _stat buf;
  if (_stat(absolute_path.path.c_str(), &buf) != 0) {
    switch (errno) {
      case ENOENT:
        // std::cerr << "GetLastModificationTime: unable to find file " <<
        // absolute_path << std::endl;
        return nullopt;
      case EINVAL:
        // std::cerr << "GetLastModificationTime: invalid param to _stat for
        // file file " << absolute_path << std::endl;
        return nullopt;
      default:
        // std::cerr << "GetLastModificationTime: unhandled for " <<
        // absolute_path << std::endl;  exit(1);
        return nullopt;
    }
  }

  return buf.st_mtime;
}

void MoveFileTo(const AbsolutePath& destination, const AbsolutePath& source) {
  MoveFile(source.path.c_str(), destination.path.c_str());
}

void CopyFileTo(const AbsolutePath& destination, const AbsolutePath& source) {
  CopyFile(source.path.c_str(), destination.path.c_str(), false /*failIfExists*/);
}

bool IsSymLink(const AbsolutePath& path) {
  return false;
}

std::vector<const char*> GetPlatformClangArguments() {
  //
  // Found by executing
  //
  //   $ clang++ -E -x c++ - -v
  //
  // https://clang.llvm.org/docs/MSVCCompatibility.html
  //
  //
  // These options are only needed if clang is targeting the msvc triple,
  // which depends on how clang was build for windows. clang downloaded from
  // releases.llvm.org defaults to msvc, so cquery does as well.
  //
  // https://github.com/cquery-project/cquery/issues/509 has more context.
  //
  return {"-fms-extensions", "-fms-compatibility",
          "-fdelayed-template-parsing"};
}

void FreeUnusedMemory() {}

bool RunObjectiveCIndexTests() {
  return false;
}

// TODO Wait for debugger to attach
void TraceMe() {}

std::string GetExternalCommandOutput(const std::vector<std::string>& command,
                                     std::string_view input) {
  return "";
}

#endif
