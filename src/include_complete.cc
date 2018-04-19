#include "include_complete.h"

#include "match.h"
#include "platform.h"
#include "project.h"
#include "standard_includes.h"
#include "timer.h"
#include "work_thread.h"

#include <thread>

namespace {

struct CompletionCandidate {
  std::string absolute_path;
  lsCompletionItem completion_item;
};

std::string ElideLongPath(const std::string& path) {
  if (g_config->completion.includeMaxPathSize <= 0)
    return path;

  if ((int)path.size() <= g_config->completion.includeMaxPathSize)
    return path;

  size_t start = path.size() - g_config->completion.includeMaxPathSize;
  return ".." + path.substr(start + 2);
}

size_t TrimCommonPathPrefix(const std::string& result,
                            const std::string& trimmer) {
  size_t i = 0;
  while (i < result.size() && i < trimmer.size()) {
    char a = result[i];
    char b = trimmer[i];
#if defined(_WIN32)
    a = (char)tolower(a);
    b = (char)tolower(b);
#endif
    if (a != b)
      break;
    ++i;
  }

  if (i == trimmer.size())
    return i;
  return 0;
}

// Returns true iff angle brackets should be used.
bool TrimPath(Project* project,
              const std::string& project_root,
              std::string* insert_path) {
  size_t start = 0;
  bool angle = false;

  size_t len = TrimCommonPathPrefix(*insert_path, project_root);
  if (len > start)
    start = len;

  for (const Directory& include_dir : project->quote_include_directories) {
    len = TrimCommonPathPrefix(*insert_path, include_dir.path);
    if (len > start)
      start = len;
  }

  for (const Directory& include_dir : project->angle_include_directories) {
    len = TrimCommonPathPrefix(*insert_path, include_dir.path);
    if (len > start) {
      start = len;
      angle = true;
    }
  }

  *insert_path = insert_path->substr(start);
  return angle;
}

lsCompletionItem BuildCompletionItem(const std::string& path,
                                     bool use_angle_brackets,
                                     bool is_stl) {
  lsCompletionItem item;
  item.label = ElideLongPath(path);
  item.detail = path;  // the include path, used in de-duplicating
  item.textEdit = lsTextEdit();
  item.textEdit->newText = path;
  item.insertTextFormat = lsInsertTextFormat::PlainText;
  item.use_angle_brackets_ = use_angle_brackets;
  if (is_stl) {
    item.kind = lsCompletionItemKind::Module;
    item.priority_ = 2;
  } else {
    item.kind = lsCompletionItemKind::File;
    item.priority_ = 1;
  }
  return item;
}

}  // namespace

IncludeComplete::IncludeComplete(Project* project)
    : is_scanning(false), project_(project) {}

void IncludeComplete::Rescan() {
  if (is_scanning)
    return;

  completion_items.clear();
  absolute_path_to_completion_item.clear();
  inserted_paths.clear();

  if (!match_ && (!g_config->completion.includeWhitelist.empty() ||
                  !g_config->completion.includeBlacklist.empty()))
    match_ =
        std::make_unique<GroupMatch>(g_config->completion.includeWhitelist,
                                     g_config->completion.includeBlacklist);

  is_scanning = true;
  WorkThread::StartThread("scan_includes", [this]() {
    Timer timer;

    InsertStlIncludes();
    InsertIncludesFromDirectory(g_config->projectRoot,
                                false /*use_angle_brackets*/);
    for (const Directory& dir : project_->quote_include_directories)
      InsertIncludesFromDirectory(dir.path, false /*use_angle_brackets*/);
    for (const Directory& dir : project_->angle_include_directories)
      InsertIncludesFromDirectory(dir.path, true /*use_angle_brackets*/);

    timer.ResetAndPrint("[perf] Scanning for includes");
    is_scanning = false;
  });
}

void IncludeComplete::InsertCompletionItem(const std::string& absolute_path,
                                           lsCompletionItem&& item) {
  if (inserted_paths.insert({item.detail, inserted_paths.size()}).second) {
    completion_items.push_back(item);
    // insert if not found or with shorter include path
    auto it = absolute_path_to_completion_item.find(absolute_path);
    if (it == absolute_path_to_completion_item.end() ||
        completion_items[it->second].detail.length() > item.detail.length()) {
      absolute_path_to_completion_item[absolute_path] =
          completion_items.size() - 1;
    }
  } else {
    lsCompletionItem& inserted_item =
        completion_items[inserted_paths[item.detail]];
    // Update |use_angle_brackets_|, prefer quotes.
    if (!item.use_angle_brackets_)
      inserted_item.use_angle_brackets_ = false;
  }
}

void IncludeComplete::AddFile(const std::string& absolute_path) {
  if (!EndsWithAny(absolute_path, g_config->completion.includeSuffixWhitelist))
    return;
  if (match_ && !match_->IsMatch(absolute_path))
    return;

  std::string trimmed_path = absolute_path;
  bool use_angle_brackets =
      TrimPath(project_, g_config->projectRoot, &trimmed_path);
  lsCompletionItem item =
      BuildCompletionItem(trimmed_path, use_angle_brackets, false /*is_stl*/);

  std::unique_lock<std::mutex> lock(completion_items_mutex, std::defer_lock);
  if (is_scanning)
    lock.lock();
  InsertCompletionItem(absolute_path, std::move(item));
  if (lock)
    lock.unlock();
}

void IncludeComplete::InsertIncludesFromDirectory(std::string directory0,
                                                  bool use_angle_brackets) {
  optional<AbsolutePath> directory = NormalizePath(directory0);
  if (!directory)
    return;

  EnsureEndsInSlash(directory->path);
  if (match_ && !match_->IsMatch(directory->path)) {
    // Don't even enter the directory if it fails the patterns.
    return;
  }

  std::vector<CompletionCandidate> results;
  GetFilesInFolder(
      directory->path, true /*recursive*/, false /*add_folder_to_path*/,
      [&](const std::string& path) {
        if (!EndsWithAny(path, g_config->completion.includeSuffixWhitelist))
          return;
        if (match_ && !match_->IsMatch(directory->path + path))
          return;

        CompletionCandidate candidate;
        candidate.absolute_path = directory->path + path;
        candidate.completion_item =
            BuildCompletionItem(path, use_angle_brackets, false /*is_stl*/);
        results.push_back(candidate);
      });

  std::lock_guard<std::mutex> lock(completion_items_mutex);
  for (CompletionCandidate& result : results)
    InsertCompletionItem(result.absolute_path,
                         std::move(result.completion_item));
}

void IncludeComplete::InsertStlIncludes() {
  std::lock_guard<std::mutex> lock(completion_items_mutex);
  for (const char* stl_header : kStandardLibraryIncludes) {
    completion_items.push_back(BuildCompletionItem(
        stl_header, true /*use_angle_brackets*/, true /*is_stl*/));
  }
}

optional<lsCompletionItem> IncludeComplete::FindCompletionItemForAbsolutePath(
    const std::string& absolute_path) {
  std::lock_guard<std::mutex> lock(completion_items_mutex);

  auto it = absolute_path_to_completion_item.find(absolute_path);
  if (it == absolute_path_to_completion_item.end())
    return nullopt;
  return completion_items[it->second];
}
