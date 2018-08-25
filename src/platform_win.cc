#if defined(_WIN32)
#include "platform.h"

#include "utils.h"

#include <loguru.hpp>

#include <windows.h>

// Has to be included before knownFolders.h
#include <initguid.h>

#include <Knownfolders.h>
#include <Objbase.h>
#include <Shlobj.h>
#include <direct.h>
#include <fcntl.h>
#include <io.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <cassert>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

namespace {
void EmitError(const std::string& message) {
  LOG_S(ERROR) << message << " (errorcode=" << GetLastError() << ")";
}
}  // namespace

void PlatformInit() {
  // We need to write to stdout in binary mode because in Windows, writing
  // \n will implicitly write \r\n. Language server API will ignore a
  // \r\r\n split request.
  _setmode(_fileno(stdout), O_BINARY);
  _setmode(_fileno(stdin), O_BINARY);

  // `clang++ -E -x c++ - -v < nul` will not find system include directories if
  // ProgramData is not set.
  if (!getenv("ProgramData")) {
    LOG_S(WARNING) << "The \"ProgramData\" environment variable is not set. "
                   << "System includes like <vector> may not be resolved "
                   << "properly.";
  }
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

optional<AbsolutePath> NormalizePath(const std::string& path0,
                                     bool ensure_exists,
                                     bool force_lower_on_windows) {
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
  if (ensure_exists) {
    len = GetLongPathName(path.c_str(), buffer, MAX_PATH);
    if (!len)
      return nullopt;
    path = std::string(buffer, len);
  }

  // Empty paths have no meaning.
  if (path.empty())
    return nullopt;

  // We may need to normalize the drive name to upper-case; at the moment
  // vscode sends lower-case path names.
  /*
  path[0] = toupper(path[0]);
  */
  // Make the path all lower-case, since windows is case-insensitive.
  if (force_lower_on_windows) {
    for (size_t i = 0; i < path.size(); ++i)
      path[i] = (char)tolower(path[i]);
  }

  // cquery assumes forward-slashes.
  std::replace(path.begin(), path.end(), '\\', '/');

  return AbsolutePath(path, false /*validate*/);
}

bool TryMakeDirectory(const AbsolutePath& absolute_path) {
  if (_mkdir(absolute_path.path.c_str()) == -1) {
    // Success if the directory exists.
    return errno == EEXIST;
  }
  return true;
}

optional<AbsolutePath> TryMakeTempDirectory(char * /* tmpl: ignored */) {
    // get "temp" dir
    char tmpdir_buf[MAX_PATH];
    DWORD len = GetTempPath(MAX_PATH, tmpdir_buf);

    // Unfortunately, there is no mkdtemp() on windows. We append a (random)
    // GUID to use as a unique directory name.
    GUID guid;
    CoCreateGuid(&guid);
    // simplest way to append the guid to the existing c string:
    len += snprintf(tmpdir_buf + len, MAX_PATH - len,
        "cquery-%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
             guid.Data1, guid.Data2, guid.Data3,
             guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
             guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

    AbsolutePath dirPath(std::string(tmpdir_buf, len));

    // finally, create the dir
    LOG_S(2) << "Creating temporary path " << dirPath;
    if(!TryMakeDirectory(dirPath))
    {
        return {};
    }

    return dirPath;
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
#if defined(_MSC_VER)
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
  CopyFile(source.path.c_str(), destination.path.c_str(),
           false /*failIfExists*/);
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

optional<std::string> GetGlobalConfigDirectory() {
  wchar_t* roaming_path = NULL;
  optional<std::string> cfg_path = {};
  if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT,
                                     NULL, &roaming_path))) {
    std::wstringstream roaming_stream;
    roaming_stream << roaming_path << L"/cquery";

    // Convert the roaming path string to a normal string so it
    // is analogous with the string returned by the posix version.
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    cfg_path = converter.to_bytes(roaming_stream.str());

    // As per the docs for SHGetKnownFolderPath
    // (https://msdn.microsoft.com/en-us/library/bb762188(VS.85).aspx)
    // we must free roaming_path using CoTaskMemFree once
    // finished with it.
    CoTaskMemFree(static_cast<void*>(roaming_path));
  }
  return cfg_path;
}

#endif
