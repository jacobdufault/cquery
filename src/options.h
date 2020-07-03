#pragma once

#include <unordered_map>
#include <string>
std::unordered_map<std::string, std::string> ParseOptions(int argc,
                                                          char** argv);

bool HasOption(const std::unordered_map<std::string, std::string>& options,
               const std::string& option);
