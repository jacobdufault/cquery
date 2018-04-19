#pragma once

#include "lsp_diagnostic.h"

#include <clang-c/Index.h>
#include <optional.h>

#include <vector>

// Builds an lsDiagnostic instance, but only if |path| matches the diagnostic
// path.
optional<lsDiagnostic> BuildAndDisposeDiagnostic(CXDiagnostic diagnostic,
                                                 const AbsolutePath& path);

// Returns the absolute path to |file|.
optional<AbsolutePath> FileName(CXFile file);

std::string ToString(CXString cx_string);

std::string ToString(CXCursorKind cursor_kind);

const char* ClangBuiltinTypeName(CXTypeKind);