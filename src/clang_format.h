#pragma once

#include "lsp.h"

#include <string>
#include <vector>

std::vector<lsTextEdit> RunClangFormat(const std::string& filename,
                                       const std::string& file_contents);

#if USE_CLANG_CXX

#include "lsp.h"
#include "working_files.h"

#include <clang/Format/Format.h>

#include <vector>

std::vector<clang::tooling::Replacement> ClangFormatDocument(
    WorkingFile* working_file,
    int start,
    int end,
    lsFormattingOptions options);

#endif
