#if defined(_WIN32)
#include "platform.h"

#include "utils.h"

#include <loguru.hpp>

#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <cassert>
#include <iostream>
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

optional<std::string> RunExecutable(const std::vector<std::string>& command,
                                    std::string_view input) {
  // FIXME: quote the first argument of command if it is not quoted and it
  // contains spaces

  // https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
  HANDLE handle_child_stdin_write = nullptr;
  HANDLE handle_child_stdin_read = nullptr;
  HANDLE handle_child_stdout_write = nullptr;
  HANDLE handle_child_stdout_read = nullptr;

  // Set the bInheritHandle flag so pipe handles are inherited.
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = true;
  saAttr.lpSecurityDescriptor = nullptr;

  // Create a pipe for the child process's STDOUT and STDIN.
  if (!CreatePipe(&handle_child_stdout_read, &handle_child_stdout_write,
                  &saAttr, 0)) {
    EmitError("CreatePipe");
    return nullopt;
  }
  // Ensure the read handle to the pipe for STDOUT is not inherited.
  if (!SetHandleInformation(handle_child_stdout_read, HANDLE_FLAG_INHERIT, 0)) {
    EmitError("SetHandleInformation handle_child_stdout_read");
    return nullopt;
  }
  if (!CreatePipe(&handle_child_stdin_read, &handle_child_stdin_write, &saAttr,
                  0)) {
    EmitError("CreatePipe");
    return nullopt;
  }
  // Ensure the write handle to the pipe for STDIN is not inherited.
  if (!SetHandleInformation(handle_child_stdin_write, HANDLE_FLAG_INHERIT, 0)) {
    EmitError("SetHandleInformation handle_child_stdin_write");
    return nullopt;
  }

  // Create a child process that uses the previously created pipes for STDIN and
  // STDOUT.
  std::string cmdline = StringJoin(command, " ");
  PROCESS_INFORMATION proc_info = {0};
  STARTUPINFO start_info = {0};
  start_info.cb = sizeof(STARTUPINFO);
  start_info.hStdInput = handle_child_stdin_read;
  start_info.hStdOutput = handle_child_stdout_write;
  start_info.hStdError = handle_child_stdout_write;
  start_info.dwFlags |= STARTF_USESTDHANDLES;

  // Create the child process.
  bool success =
      CreateProcess(nullptr,                             // application name
                    const_cast<char*>(cmdline.c_str()),  // command line
                    nullptr,      // process security attributes
                    nullptr,      // primary thread security attributes
                    true,         // handles are inherited
                    0,            // creation flags
                    nullptr,      // use parent's environment
                    nullptr,      // use parent's current directory
                    &start_info,  // STARTUPINFO pointer
                    &proc_info);  // receives PROCESS_INFORMATION
  if (!success) {
    EmitError("CreateProcess");
    return nullopt;
  }

  // Write the the child stdin.
  const char* start = input.data();
  size_t remaining = input.length();
  while (remaining > 0) {
    DWORD written;
    bool success = WriteFile(handle_child_stdin_write, start, remaining,
                             &written, nullptr);
    if (!success) {
      EmitError("WriteFile");
      return nullopt;
    }
    remaining -= written;
  }
  // Make sure to close the stdin handle after writing stdin, otherwise the
  // child may block indefinately since it thinks it may have more input.
  if (!CloseHandle(handle_child_stdin_write)) {
    EmitError("CloseHandle handle_child_stdin_write");
    return nullopt;
  }

  // Read all of the content in the child stdout pipe.
  std::string output;
  auto read_from_stdout_pipe = [&]() {
    while (true) {
      constexpr int kBufSize = 4096;

      // If there is nothing available in the child pipe, ReadFile will block
      // (possibly forever if the child has exited).
      DWORD bytes_available = 0;
      if (!PeekNamedPipe(handle_child_stdout_read, nullptr, 0, nullptr,
                         &bytes_available, nullptr)) {
        EmitError("PeekNamedPipe");
      }
      if (bytes_available == 0)
        break;

      DWORD bytes_read;
      char buffer[kBufSize];
      bool success = ReadFile(handle_child_stdout_read, buffer, kBufSize,
                              &bytes_read, NULL);
      if (!success || bytes_read == 0)
        break;

      for (unsigned long i = 0; i < bytes_read; ++i)
        output += buffer[i];
    }
  };
  // Wait for the process to finish. While running consume output from the pipe
  // so it does not block.
  while (true) {
    // Keep looping until the process exits
    if (WaitForSingleObject(proc_info.hProcess, 0) == WAIT_OBJECT_0)
      break;
    read_from_stdout_pipe();
  }
  read_from_stdout_pipe();

  // Close handles.
  if (!CloseHandle(handle_child_stdin_read))
    EmitError("handle_child_stdin_read");
  if (!CloseHandle(handle_child_stdout_write))
    EmitError("handle_child_stdout_write");
  if (!CloseHandle(handle_child_stdout_read))
    EmitError("CloseHandle handle_child_stdout_read");
  if (!CloseHandle(proc_info.hProcess))
    EmitError("CloseHandle proc_info.hProcess");
  if (!CloseHandle(proc_info.hThread))
    EmitError("CloseHandle proc_info.hThread");

  return output;
}

#endif
