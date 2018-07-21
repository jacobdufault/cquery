#include "platform.h"
#include "utils.h"

#include <doctest/doctest.h>
#include <loguru.hpp>
#include <reproc/parser.hpp>
#include <reproc/reproc.hpp>

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

optional<std::string> RunExecutable(const std::vector<std::string>& command,
                                    std::string_view input) {
  using reproc::Reproc;

  std::string command_string = "\"" + StringJoin(command, " ") + "\"";
  auto command_with_error = [command_string](reproc::Error error) {
    return command_string + ": " + reproc::error_to_string(error);
  };

  Reproc reproc;
  reproc::Error error = reproc::SUCCESS;

  error = reproc.start(command, nullptr);
  if (error) {
    LOG_S(ERROR) << "Error starting reproc " << command_with_error(error);
    return nullopt;
  }

  unsigned int bytes_written = 0;
  error = reproc.write(input.data(), input.length(), &bytes_written);
  if (error) {
    LOG_S(ERROR) << "Error writing to stdin of " << command_with_error(error)
                 << ". " << bytes_written << " out of " << input.length()
                 << " bytes were written";
    return nullopt;
  }

  error = reproc.close(reproc::STDIN);
  if (error) {
    LOG_S(ERROR) << "Error closing stdin of " << command_with_error(error);
    return nullopt;
  }

  std::string output{};
  error = reproc.read(reproc::STDOUT, reproc::string_parser(output));
  if (error) {
    LOG_S(ERROR) << "Error reading output of " << command_with_error(error);
    return nullopt;
  }

  error = reproc.read(reproc::STDERR, reproc::string_parser(output));
  if (error) {
    LOG_S(ERROR) << "Error reading stderr output of "
                 << command_with_error(error);
    return nullopt;
  }

  error = reproc.wait(reproc::INFINITE);
  if (error) {
    LOG_S(ERROR) << "Error waiting for exit of " << command_with_error(error);
    return nullopt;
  }

  int exit_status = 0;
  error = reproc.exit_status(&exit_status);
  if (error) {
    LOG_S(ERROR) << "Error retrieving exit status of "
                 << command_with_error(error);
    return nullopt;
  }

  if (exit_status != 0) {
    LOG_S(ERROR) << command_string << " failed with exit status "
                 << exit_status;
  }

  LOG_S(INFO) << "Executed " << command_string;

  return output;
}

TEST_SUITE("Platform") {
  TEST_CASE("Split strings") {
    std::vector<std::string> actual = Split("/a/b/c/", '/');
    std::vector<std::string> expected{"a", "b", "c"};
    REQUIRE(actual == expected);
  }
}
