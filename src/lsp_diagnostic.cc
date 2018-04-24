#include "lsp_diagnostic.h"

bool lsDiagnostic::operator==(const lsDiagnostic& rhs) const {
  // Just check the important fields.
  return range == rhs.range && message == rhs.message;
}
bool lsDiagnostic::operator!=(const lsDiagnostic& rhs) const {
  return !(*this == rhs);
}