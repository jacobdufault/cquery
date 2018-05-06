#pragma once

#include <string>
#include <vector>

// Used to identify the compiler type.
enum class CompilerType {
  Unknown = 0,
  Clang = 1,
  GCC = 2,
  MSVC = 3,
};

// Find out the compiler type for the specific driver.
CompilerType FindCompilerType(const std::string& compiler_driver);

// Whether the compiler accepts certain flag.
bool CompilerAcceptsFlag(CompilerType compiler_type, const std::string& flag);

// Append flag if the compiler accepts it.
void CompilerAppendsFlagIfAccept(CompilerType compiler_type,
                                 const std::string& flag,
                                 std::vector<std::string>& flags);
