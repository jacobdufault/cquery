#include "platform.h"
#include "utils.h"

#include <doctest/doctest.h>
#include <loguru.hpp>
#include <process.hpp>

#include <iterator>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace {

// See http://stackoverflow.com/a/236803
template <typename Out>
void Split(const std::string& s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (!item.empty())
      *(result++) = item;
  }
}
std::vector<std::string> Split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  Split(s, delim, std::back_inserter(elems));
  return elems;
}

std::string Join(const std::vector<std::string>& entries,
                 char delim,
                 size_t end) {
  std::string result;
  bool first = true;
  for (size_t i = 0; i < end; ++i) {
    if (!first)
      result += delim;
    first = false;
    result += entries[i];
  }
  return result;
}

}  // namespace

PlatformMutex::~PlatformMutex() = default;

PlatformScopedMutexLock::~PlatformScopedMutexLock() = default;

PlatformSharedMemory::~PlatformSharedMemory() = default;

void MakeDirectoryRecursive(const AbsolutePath& path) {
  if (TryMakeDirectory(path))
    return;

  std::string prefix = "";
  if (path.path[0] == '/')
    prefix = "/";

  std::vector<std::string> components = Split(path.path, '/');

  // Find first parent directory which doesn't exist.
  int first_success = -1;
  for (size_t j = 0; j < components.size(); ++j) {
    size_t i = components.size() - j;
    if (TryMakeDirectory(AbsolutePath(prefix + Join(components, '/', i)))) {
      first_success = i;
      break;
    }
  }

  if (first_success == -1) {
    LOG_S(FATAL) << "Failed to make any parent directory for " << path;
    exit(1);
  }

  // Make all child directories.
  for (size_t i = first_success + 1; i <= components.size(); ++i) {
    if (TryMakeDirectory(AbsolutePath(prefix + Join(components, '/', i))) ==
        false) {
      LOG_S(FATAL) << "Failed making directory for " << path
                   << " even after creating parent directories";
      exit(1);
    }
  }
}
#include <iostream>
optional<std::string> RunExecutable(const std::vector<std::string>& command,
                                    std::string_view input) {
  Process::Error error;
  Process process;

  for (const auto& arg : command) {
    std::cerr << arg << " ";
  }
  std::cerr << std::endl;

  error = process.start(command, nullptr);
  if (error) {
    std::string command_string = StringJoin(command);
    LOG_S(ERROR) << "Process: Failed to start process \"" << command_string
                 << "\"";
    return nullopt;
  }

  unsigned int actual;

  error = process.write(input.data(), input.length(), &actual);
  if (error || actual != input.length()) {
    LOG_S(FATAL) << "Process: Failed to write input";
    return nullopt;
  }

  error = process.close_stdin();
  if (error) {
    LOG_S(FATAL) << "Process: Failed to close stdin";
    return nullopt;
  }

  char buffer[1024];
  unsigned int buffer_size = 1024;

  std::stringstream output;

  while (true) {
    error = process.read(buffer, buffer_size - 1, &actual);
    if (error) {
      break;
    }

    buffer[actual] = '\0';
    output << buffer;
  }

  if (error != Process::STREAM_CLOSED) {
    LOG_S(FATAL) << "Process: Failed to read output";
    return nullopt;
  }

  error = process.wait(Process::INFINITE);
  if (error) {
    LOG_S(FATAL) << "Process: Failed to wait for process exit";
    return nullopt;
  }

  return output.str();
}

TEST_SUITE("Platform") {
  TEST_CASE("Split strings") {
    std::vector<std::string> actual = Split("/a/b/c/", '/');
    std::vector<std::string> expected{"a", "b", "c"};
    REQUIRE(actual == expected);
  }
}
