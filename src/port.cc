#include "port.h"

#include <stdio.h>
#include <stdlib.h>

void cquery_unreachable_internal(const char* file, int line) {
  fprintf(stderr, "unreachable %s:%d\n", file, line);
  CQUERY_BUILTIN_UNREACHABLE;
  abort();
}
