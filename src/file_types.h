#pragma once

#include "hash_utils.h"
#include "serializer.h"

#include <iosfwd>
#include <string>

struct AbsolutePath {
  static AbsolutePath BuildDoNotUse(std::string_view path);

  // Try not to use this.
  AbsolutePath();

  // Provide implicit conversions to std::string for the time being.
  AbsolutePath(const std::string& path, bool validate = true);
  operator std::string() const;

  bool operator==(const AbsolutePath& rhs) const;
  bool operator!=(const AbsolutePath& rhs) const;

  std::string path;
};
MAKE_HASHABLE(AbsolutePath, t.path);

void Reflect(Reader& visitor, AbsolutePath& value);
void Reflect(Writer& visitor, AbsolutePath& value);

std::ostream& operator<<(std::ostream& out, const AbsolutePath& path);

struct Directory {
  explicit Directory(const AbsolutePath& path);

  bool operator==(const Directory& rhs) const;
  bool operator!=(const Directory& rhs) const;

  std::string path;
};
MAKE_HASHABLE(Directory, t.path);