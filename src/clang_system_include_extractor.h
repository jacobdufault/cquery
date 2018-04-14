#pragma once

#include <string>
#include <vector>

// Run clang specified by `clang_binary` and return the set of system includes
// it uses.
std::vector<std::string> FindSystemIncludeDirectories(
    const std::string& clang_binary,
    const std::string& language,
    const std::string& working_directory,
    const std::vector<std::string>& extra_flags);
