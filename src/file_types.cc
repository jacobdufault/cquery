#include "file_types.h"

#include "utils.h"
#include "platform.h"

// static
AbsolutePath AbsolutePath::Build(const std::string& path) {
  // TODO: add a debug-only check here that this is actually normalized.
  return AbsolutePath(path);
}

AbsolutePath::AbsolutePath() {}

AbsolutePath::AbsolutePath(const std::string& path)
  : path(path) {}

AbsolutePath::operator std::string() const {
  return path;
}

bool AbsolutePath::operator==(const AbsolutePath& rhs) const {
  return path == rhs.path;
}

bool AbsolutePath::operator!=(const AbsolutePath& rhs) const {
  return path != rhs.path;
}

Directory::Directory(const std::string& path) : Directory(AbsolutePath(path)) {}

Directory::Directory(const AbsolutePath& path) : path(path) {
  EnsureEndsInSlash(this->path.path);
}

Directory::operator std::string() const {
  return path;
}

bool Directory::operator==(const Directory& rhs) const {
  return path == rhs.path;
}

bool Directory::operator!=(const Directory& rhs) const {
  return path != rhs.path;
}