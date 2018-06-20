#pragma once

#include <string>
#include <vector>

#include "optional.h"

struct CCppProperties {
  std::string cstd;
  std::string cppstd;
  std::vector<std::string> args;
};

optional<CCppProperties> LoadCCppProperties(
    const std::string& json_full_path,
    const std::string& project_dir);
