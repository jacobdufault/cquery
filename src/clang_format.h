#pragma once

#include "lsp.h"

#include <string>
#include <vector>

std::vector<lsTextEdit> RunClangFormat(const std::string& filename,
                                       const std::string& file_contents,
                                       optional<int> start_offset,
                                       optional<int> end_offset);