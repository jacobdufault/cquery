#include <loguru.hpp>
#include "message_handler.h"
#include "query_utils.h"
#include "queue_manager.h"

namespace {

/// Trim the spaces and special comment markup at the beginning of the string.
static std::string ltrim(std::string s) {
  auto it = s.begin();
  for (; it != s.end() && isspace(*it); ++it) {
  }

  // Is it a // comment? Then do not strip anything right after it except for
  // spaces.
  if (std::distance(it, s.end()) >= 1 && *it == '/' && *std::next(it) == '/') {
    ++it;
    ++it;
    for (; it != s.end() && *it == '/'; ++it) {
    }

    // for (; it != s.end() && isspace(*it); ++it) {
    //}

    if (it == s.end())
      return "";
    return std::string(it, s.end());
  }

  bool star_or_excl_was_seen = false;
  for (; it != s.end() &&
         (isspace(*it) || *it == '*' || *it == '/' || *it == '!');
       ++it) {
    if (*it == '*' || *it == '!')
      star_or_excl_was_seen = true;
    // Do not allow for / after stars or exclamation marks.
    if (*it == '/' && star_or_excl_was_seen)
      break;
  }

  if (it == s.end())
    return "";
  return std::string(it, s.end());
}

/// Trim the spaces and special comment markup at the end of the string. Leave
/// only one space at the end.
static std::string rtrim(std::string s) {
  char last = '\0';
  if (s.empty())
    return s;
  auto it = std::prev(s.end());
  while ((isspace(*it) || *it == '*' || *it == '/') && it != s.begin()) {
    last = *it;
    --it;
  }

  if (!last)
    return s;
  // Leave one space at the end.
  if (isspace(last))
    ++it;
  return std::string(s.begin(), it);
}

/// Split the string using a provided \p delimiter.
std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

/// Join strings using a provided \p delimiter.
static std::string join(std::vector<std::string> strings,
                        const char* delimiter) {
  std::ostringstream os;
  bool first = true;
  std::string last;
  for (auto& s : strings) {
    // Skip leading empty lines.
    if (s.empty())
      continue;
    // Check if current string starts with space*[-|*|digit],
    // because it is a special markup.
    auto it = s.begin();
    for (; it != s.end() && isspace(*it); ++it) {
    }
    bool is_markdown = false;
    if (it != s.end() && (*it == '-' || *it == '*' || isdigit(*it)))
      is_markdown = true;
    // Do not use a delimiter after newlines.
    if (!first && last != "\n\n" && !is_markdown) {
      os << delimiter;
    }
    // Do not produce sequences of multiple newlines.
    if (last == "\n\n" && s == "\n\n")
      continue;
    if (is_markdown && !last.empty() && last.back() != '\n')
      os << "\n";
    os << s;
    last = s;
    first = false;
  }
  return os.str();
}

void replace_all(std::string& line,
                 const std::string& oldString,
                 const std::string& newString) {
  const size_t oldSize = oldString.length();

  // do nothing if line is shorter than the string to find
  if (oldSize > line.length())
    return;

  const size_t newSize = newString.length();
  for (size_t pos = 0;; pos += newSize) {
    // Locate the substring to replace
    pos = line.find(oldString, pos);
    if (pos == std::string::npos)
      return;
    if (oldSize == newSize) {
      // if they're same size, use std::string::replace
      line.replace(pos, oldSize, newString);
    } else {
      // if not same size, replace by erasing and inserting
      line.erase(pos, oldSize);
      line.insert(pos, newString);
    }
  }
}

/// Represents a section in the comment. This section may be a named section
/// started by special commands like e.g. \param or @param or it can be an
/// unnamed section corresponding to e.g. a paragraph in the comment.
struct CommentSection {
  /// Name of the section, e.g. param, returns, exceptions.
  /// If it is empty, it is an unnamed section, e.g. a paragraph in the original
  /// comment.
  std::string section_name;
  /// Mode of the parameter. Possible values are in, out and inout.
  std::string mode;
  /// Name of the parameter.
  std::string name;
  /// Description of the section.
  std::vector<std::string> description;
  /// Where the section description starts in the original line.
  std::string::const_iterator description_begin;
  CommentSection() = default;
  CommentSection(const std::string& s) { description.push_back(s); }
};

using CommentSections = std::vector<CommentSection>;

// TODO: Support more special commands?
static std::vector<std::string> known_comment_section_names{
    "param", "returns", "return", "exceptions"};

static std::vector<std::string> known_param_modes{"in", "out", "inout"};

/// Process a comment line starting at the position \p pos of \p input_line.
/// \returns true if a new comment section was found (e.g. a section starting
/// with \param).
static bool process_comment_line(const std::string& input_line,
                                 std::string::size_type pos,
                                 CommentSection& comment_section) {
  const std::string line = input_line.substr(pos);
  comment_section = CommentSection(line);
  comment_section.description_begin = input_line.begin() + pos;
  if (line.empty() || line == "\n\n" || line == "\n")
    return false;
  auto it = line.begin();
  // Skip spaces.
  while (it != line.end() && isspace(*it)) {
    ++it;
  }

  // Support both \ and @ as symbols starting a section.
  if (*it != '\\' && *it != '@')
    return false;
  ++it;
  auto section_name_begin = it;
  // Read the name of the section.
  while (it != line.end() && (isalpha(*it) || *it == '_' || *it == '.')) {
    ++it;
  }
  auto section_name = std::string(section_name_begin, it);

  LOG_S(INFO) << "Found section: " << section_name << "\n";
  // Bail if it is an unknown section. Consider this line to be just a regular
  // line.
  // TODO: Support more commands?
  if (std::find(known_comment_section_names.begin(),
                known_comment_section_names.end(),
                section_name) == known_comment_section_names.end()) {
    LOG_S(INFO) << "Unknown comment section: " << section_name << "\n";
    return false;
  }

  // Skip spaces after the section name.
  while (it != line.end() && isspace(*it)) {
    ++it;
  }

  comment_section.section_name = section_name;

  std::string::const_iterator description_begin = it;
  std::string param_mode;
  // Process param sections.
  if (section_name == "param") {
    // Parse parameter mode: [in|out|inout]
    if (it != line.end() && *it == '[') {
      ++it;
      // Skip spaces.
      while (it != line.end() && isspace(*it)) {
        ++it;
      }
      auto param_mode_begin = it;
      while (it != line.end() && isalpha(*it)) {
        ++it;
      }
      param_mode = std::string(param_mode_begin, it);
      // Skip spaces.
      while (it != line.end() && isspace(*it)) {
        ++it;
      }
      if (it != line.end() && *it == ']') {
        ++it;
        if (std::find(known_param_modes.begin(), known_param_modes.end(),
                      param_mode) != known_param_modes.end()) {
          // Skip spaces.
          while (it != line.end() && isspace(*it)) {
            ++it;
          }
          description_begin = it;
        } else {
          // Implicit in.
          param_mode = "in";
        }
      }
    } else {
      // Implicit in.
      param_mode = "in";
    }

    comment_section.mode = param_mode;

    // Read parameter name
    it = description_begin;
    auto param_name_begin = it;
    // Read the name of the section.
    while (it != line.end() && !isspace(*it)) {
      ++it;
    }
    auto param_name = std::string(param_name_begin, it);
    comment_section.name = param_name;
    // Skip spaces.
    while (it != line.end() && isspace(*it)) {
      ++it;
    }
    description_begin = it;
  }

  // Store the rest of the line as a description of this section.
  comment_section.description.clear();
  comment_section.description.push_back(
      std::string(description_begin, line.end()));
  comment_section.description_begin =
      input_line.begin() + std::distance(line.begin(), description_begin) + pos;
  return true;
}

/// Format collected comment sections.
/// Order them properly and format them in a nice way.
/// Let's form two CommentSections:
///  - one for a textual part of the comment
///  - and another one for a parameter/returns/exceptions descriptions
static void format_comment_sections(CommentSections& sections) {
  CommentSections formatted_sections;
  // Merge all lines inside each section.
  for (auto& section : sections) {
    // First remove trailing \n lines in each section.
    while (section.description.back() == "\n" ||
           section.description.back() == "\n\n")
      section.description.pop_back();
    // Then merge lines inside the current section.
    std::string combined = join(section.description, " ");
    section.description.clear();
    section.description.push_back(combined);
  }

  // Merge all unnamed sections together.
  std::string unnamed_sections_combined;
  for (auto& section : sections) {
    if (!section.section_name.empty())
      continue;
    if (unnamed_sections_combined.empty()) {
      unnamed_sections_combined = section.description[0];
      continue;
    }
    // There should be at most one newline between two sections.
    unnamed_sections_combined +=
        ((*unnamed_sections_combined.rbegin() == '\n') ? "\n" : "\n\n") +
        section.description[0];
  }
  // For Markdown replace all \n with space space as it means a new line in
  // Markdown.
  replace_all(unnamed_sections_combined, "\n", "  \n");
  formatted_sections.push_back(CommentSection(unnamed_sections_combined));

  // Merge all named sections together.
  // TODO: Sort named sessions to ensure that the order is always: parameters,
  // exceptions and then returns?
  std::string named_sections_combined;
  for (auto& section : sections) {
    if (section.section_name.empty())
      continue;
    if (named_sections_combined.empty()) {
      named_sections_combined =
          "Parameters description:\n" + section.description[0];
      continue;
    }
    // There should be at most one newline between two sections.
    named_sections_combined += "\n" + section.description[0];
  }

  CommentSection parameters_description_section;
  parameters_description_section.section_name = "Parameters description";
  parameters_description_section.description.push_back(named_sections_combined);
  formatted_sections.push_back(parameters_description_section);

  sections = formatted_sections;
}

static void collect_comment_sections(const std::vector<std::string>& lines,
                                     CommentSections& sections) {
  for (const auto& line : lines) {
    CommentSection comment_section;
    // Process line from the beginning.
    process_comment_line(line, 0, comment_section);

    CommentSections comment_sections{comment_section};

    // Process parsed sections.
    while (!comment_sections.empty()) {
      assert(comment_sections.size() == 1);
      auto current_section = *comment_sections.begin();
      comment_sections.pop_back();
      // It could happen that the description contains further comment
      // sections definitions.
      auto it = current_section.description_begin;
      std::string::size_type pos = std::distance(line.begin(), it);
      do {
        if (line.empty())
          break;
        // Is there anything that looks like a start of a new comment section?
        pos = line.find_first_of("\\@", pos);
        if (pos != std::string::npos) {
          // Try to parse it to see if it is really a new comment section.
          CommentSection another_section;
          // Is this really another section?
          if (process_comment_line(line, pos, another_section)) {
            // The other comment section should be processed next.
            comment_sections.push_back(another_section);
            // Update the last line of the current section.
            current_section.description.back() = std::string(
                current_section.description_begin, line.begin() + pos);
            break;
          }
          // Continue to check if there are other candidates.
          pos++;
        }
      } while (pos != std::string::npos && pos < line.size());
      // Store information about the current section.
      if (current_section.section_name.empty()) {
        // This is an unnamed section.
        if (!sections.empty()) {
          // If the last line of the last seen section is not an empty/new line,
          // then just add this line to it.
          auto& last_line = sections.back().description.back();
          if (last_line != "\n" && last_line != "\n\n") {
            sections.back().description.push_back(
                current_section.description[0]);
            continue;
          }
        }
      } else {
        // This is a named section.
        // Create a line for this section. This line has the following format:
        // section_name [in|out|inout] [name] the reset of the description.
        std::string prefix =
            std::string{"  * "} + "**" + current_section.section_name + "** ";
        if (!current_section.mode.empty())
          prefix += "[" + current_section.mode + "] ";
        if (!current_section.name.empty())
          prefix += "**" + current_section.name + "** ";
        // Format a description for a named section.
        auto description = current_section.description.back();
        current_section.description.clear();
        current_section.description.push_back(prefix);
        current_section.description.push_back(description);
      }

      sections.push_back(current_section);
    }
  }
}

/// Format comments for presenting them in a nice way.
/// FIXME: This is a very simplistic approach to parse comments.
/// In the future, a more advanced real comment parser may need
/// to be used.
static void format_comments(const std::string& comments,
                            CommentSections& sections) {
  // Split the comments into lines and trim them on the left.
  auto lines = split(comments, '\n');

  // Trim each line on both sides.
  for (auto& line : lines) {
    line = ltrim(line);
    line = rtrim(line);
    if (line.empty())
      line = "\n\n";
  }

  // Remove leading new lines.
  for (auto s = lines.begin(), e = lines.end(); s != e; ++s) {
    if (*s != "\n\n")
      break;
    *s = "";
  }

  // Remove final new lines.
  while (lines.back() == "\n\n")
    lines.pop_back();

  // Create parameter and return info.
  collect_comment_sections(lines, sections);
  format_comment_sections(sections);
}

/// Create documentation string and hover.
/// \returns results are stored in the output parameter \p result.
static void getDocAndHover(const optional<std::string>& comments,
                           const optional<std::string>& hover,
                           CommentSections& sections) {
  if (comments)
    format_comments(*comments, sections);

  // Add the hover as the last section.
  sections.push_back(CommentSection(*hover));
}

static const optional<std::string>& getCommentOrOverriddenComment(
    QueryDatabase* db,
    QueryFunc& func) {
  if (func.def->comments && !func.def->comments->empty())
    return func.def->comments;
  for (const auto& base : func.def->base) {
    QueryFunc& base_func = db->funcs[base.id];
    if (!base_func.def)
      continue;
    if (base_func.def->comments && !base_func.def->comments->empty()) {
      return base_func.def->comments;
    }
  }
  return func.def->comments;
}

void GetHoverForSymbol(QueryDatabase* db,
                       const SymbolIdx& symbol,
                       CommentSections& sections) {
  switch (symbol.kind) {
    case SymbolKind::Type: {
      QueryType& type = db->types[symbol.idx];
      if (type.def)
        getDocAndHover(type.def->comments,
                       type.def->hover.value_or(type.def->detailed_name),
                       sections);
      break;
    }
    case SymbolKind::Func: {
      QueryFunc& func = db->funcs[symbol.idx];
      if (func.def) {
        getDocAndHover(getCommentOrOverriddenComment(db, func),
                       func.def->hover.value_or(func.def->detailed_name),
                       sections);
      }
      break;
    }
    case SymbolKind::Var: {
      QueryVar& var = db->vars[symbol.idx];
      if (var.def)
        getDocAndHover(var.def->comments,
                       var.def->hover.value_or(var.def->detailed_name),
                       sections);
      break;
    }
    case SymbolKind::File:
    case SymbolKind::Invalid: {
      assert(false && "unexpected");
      break;
    }
  }
}

struct Ipc_TextDocumentHover : public RequestMessage<Ipc_TextDocumentHover> {
  const static IpcId kIpcId = IpcId::TextDocumentHover;
  lsTextDocumentPositionParams params;
};
MAKE_REFLECT_STRUCT(Ipc_TextDocumentHover, id, params);
REGISTER_IPC_MESSAGE(Ipc_TextDocumentHover);

struct Out_TextDocumentHover : public lsOutMessage<Out_TextDocumentHover> {
  struct Result {
    std::vector<lsMarkedString> contents;
    optional<lsRange> range;
  };

  lsRequestId id;
  optional<Result> result;
};
MAKE_REFLECT_STRUCT(Out_TextDocumentHover::Result, contents, range);
void Reflect(Writer& visitor, Out_TextDocumentHover& value) {
  REFLECT_MEMBER_START();
  REFLECT_MEMBER(jsonrpc);
  REFLECT_MEMBER(id);
  if (value.result)
    REFLECT_MEMBER(result);
  else {
    // Empty optional<> is elided by the default serializer, we need to write
    // |null| to be compliant with the LSP.
    visitor.Key("result");
    visitor.Null();
  }
  REFLECT_MEMBER_END();
}

struct TextDocumentHoverHandler : BaseMessageHandler<Ipc_TextDocumentHover> {
  void Run(Ipc_TextDocumentHover* request) override {
    QueryFile* file;
    if (!FindFileOrFail(db, project, request->id,
                        request->params.textDocument.uri.GetPath(), &file)) {
      return;
    }

    WorkingFile* working_file =
        working_files->GetFileByFilename(file->def->path);

    Out_TextDocumentHover out;
    out.id = request->id;

    for (const SymbolRef& ref :
         FindSymbolsAtLocation(working_file, file, request->params.position)) {
      // Found symbol. Return hover.
      optional<lsRange> ls_range = GetLsRange(
          working_files->GetFileByFilename(file->def->path), ref.loc.range);
      if (!ls_range)
        continue;

      // Produce two output strings. One for the documentation and one for the
      // hover containing the signature/description.
      CommentSections sections;
      GetHoverForSymbol(db, ref.idx, sections);

      if (!sections.empty()) {
        // Return a separate MarkedString for each section.
        out.result = Out_TextDocumentHover::Result();

        // First string is taken from the last section containing the actual
        // hover/signature.
        if (!sections.back().description.empty()) {
          out.result->contents.emplace_back(lsMarkedString1{
              file->def->language, *sections.back().description.begin()});
        }

        // First sections contain the documentation and eventually parameter
        // descriptions.
        for (size_t idx = 0, e = sections.size() - 1; idx != e; ++idx) {
          auto& section = sections[idx];
          if (!section.description.empty()) {
            out.result->contents.emplace_back(*section.description.begin());
          }
        }
      }

      out.result->range = *ls_range;
      break;
    }

    // std::ostringstream sstream;
    // out.Write(sstream);
    // LOG_S(INFO) << sstream.str() << "\n";

    QueueManager::WriteStdout(IpcId::TextDocumentHover, out);
  }
};
REGISTER_MESSAGE_HANDLER(TextDocumentHoverHandler);
}  // namespace
