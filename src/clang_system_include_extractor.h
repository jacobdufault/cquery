#pragma once

#include <string>
#include <vector>

// Run clang specified by `clang_binaries` and return the set of system includes
// it uses.
//
// If the first entry in `clang_binaries` fails, the second is tried, and
// so-forth, until one succeeds.
std::vector<std::string> FindSystemIncludeDirectories(
    const std::vector<std::string>& compiler_drivers,
    const std::string& language,
    const std::string& working_directory,
    const std::vector<std::string>& extra_flags);
