#include "compiler.h"

#include "platform.h"
#include "utils.h"

#include <doctest/doctest.h>

#include <unordered_map>

namespace {
CompilerType ExtractCompilerType(const std::string& version_output) {
  if (version_output.find("Apple LLVM version") != std::string::npos)
    return CompilerType::Clang;
  if (version_output.find("clang version") != std::string::npos)
    return CompilerType::Clang;
  if (version_output.find("GCC") != std::string::npos)
    return CompilerType::GCC;
  if (version_output.find("Microsoft (R)") != std::string::npos)
    return CompilerType::MSVC;
  return CompilerType::Unknown;
}

// FIXME: Make FindCompilerType a class so this is not a global.
std::unordered_map<std::string, CompilerType> compiler_type_cache_;

}  // namespace

CompilerType FindCompilerType(const std::string& compiler_driver) {
  auto it = compiler_type_cache_.find(compiler_driver);
  if (it != compiler_type_cache_.end())
    return it->second;

  std::vector<std::string> flags = {compiler_driver};
  if (!EndsWith(compiler_driver, "cl.exe"))
    flags.push_back("--version");
  optional<std::string> version_output = RunExecutable(flags, "");
  CompilerType result = CompilerType::Unknown;
  if (version_output)
    result = ExtractCompilerType(version_output.value());

  compiler_type_cache_[compiler_driver] = result;
  return result;
}

bool CompilerAcceptsFlag(CompilerType compiler_type, const std::string& flag) {
  // MSVC does not accept flag beginning with '-'.
  if (compiler_type == CompilerType::MSVC && StartsWith(flag, "-"))
    return false;

  // These flags are for clang only.
  if (StartsWith(flag, "-working-directory") ||
      StartsWith(flag, "-resource-dir") || flag == "-fparse-all-comments")
    return compiler_type == CompilerType::Clang;

  return true;
}

void CompilerAppendsFlagIfAccept(CompilerType compiler_type,
                                 const std::string& flag,
                                 std::vector<std::string>& flags) {
  if (CompilerAcceptsFlag(compiler_type, flag))
    flags.emplace_back(flag);
}

TEST_SUITE("Compiler type extraction") {
  TEST_CASE("Apple Clang") {
    std::string version_output =
        "Apple LLVM version 9.1.0 (clang-902.0.39.1)\n"
        "Target: x86_64-apple-darwin17.5.0\n"
        "Thread model: posix\n"
        "InstalledDir: "
        "/Applications/Xcode.app/Contents/Developer/Toolchains/"
        "XcodeDefault.xctoolchain/usr/bin\n";
    REQUIRE(CompilerType::Clang == ExtractCompilerType(version_output));
  }
  TEST_CASE("LLVM Clang") {
    std::string version_output =
        "clang version 6.0.0 (tags/RELEASE_600/final)\n"
        "Target: x86_64-apple-darwin17.5.0\n"
        "Thread model: posix\n"
        "InstalledDir: /usr/local/opt/llvm/bin\n";
    REQUIRE(CompilerType::Clang == ExtractCompilerType(version_output));
  }
  TEST_CASE("GCC") {
    std::string version_output =
        "gcc-8 (Homebrew GCC 8.1.0) 8.1.0\n"
        "Copyright (C) 2018 Free Software Foundation, Inc.\n"
        "This is free software; see the source for copying conditions.  There "
        "is NO\n"
        "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR "
        "PURPOSE.\n";
    REQUIRE(CompilerType::GCC == ExtractCompilerType(version_output));
  }
  TEST_CASE("MSVC") {
    std::string version_output =
        "Microsoft (R) C/C++ Optimizing Compiler Version 19.00.24210 for x64\n"
        "Copyright (C) Microsoft Corporation.  All rights reserved.\n"
        "\n"
        "usage: cl [ option... ] filename... [ /link linkoption... ]\n";
    REQUIRE(CompilerType::MSVC == ExtractCompilerType(version_output));
  }
  TEST_CASE("Unknown") {
    std::string version_output = "";
    REQUIRE(CompilerType::Unknown == ExtractCompilerType(version_output));
  }
}
