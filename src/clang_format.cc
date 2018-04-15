#include "clang_format.h"

#include "lex_utils.h"
#include "platform.h"
#include "utils.h"

#include <doctest/doctest.h>
#include <loguru.hpp>
#include <pugixml.hpp>

namespace {

struct Replacement {
  int offset = 0;
  int length = 0;
  std::string text;
};

// Parses the output of `clang-format -output-replacements-xml`.
std::vector<Replacement> ParseClangFormatReplacements(const std::string& xml) {
  //
  // Output looks like this:
  //
  //   <?xml version='1.0'?>
  //   <replacements xml:space='preserve' incomplete_format='false'>
  //   <replacement offset='780' length='82'>abc</replacement>
  //   <replacement offset='1448' length='0'>def</replacement>
  //   </replacements>
  //
  std::vector<Replacement> result;

  pugi::xml_document doc;
  unsigned int flags = pugi::parse_default;
  // parse the space in <replacement>  </replacement>
  flags |= pugi::parse_ws_pcdata_single;
  doc.load_string(xml.c_str(), flags);
  for (auto child : doc.child("replacements").children("replacement")) {
    Replacement r;
    r.offset = child.attribute("offset").as_int();
    r.length = child.attribute("length").as_int();
    r.text = child.text().as_string();
    result.push_back(r);
  }

  return result;
}

std::vector<lsTextEdit> ConvertReplacementsToTextEdits(
    const std::string& document,
    const std::vector<Replacement>& replacements) {
  std::vector<lsTextEdit> edits;
  for (const Replacement& replacement : replacements) {
    lsTextEdit edit;
    edit.range.start = GetPositionForOffset(replacement.offset, document);
    edit.range.end =
        GetPositionForOffset(replacement.offset + replacement.length, document);
    edit.newText = replacement.text;
    LOG_S(INFO) << "Text edit from " << edit.range.start.ToString() << " to "
                << edit.range.end.ToString() << " with text |" << edit.newText
                << "|";
    edits.push_back(edit);
  }
  return edits;
}

}  // namespace

std::vector<lsTextEdit> RunClangFormat(const std::string& filename,
                                       const std::string& file_contents,
                                       optional<int> start_offset,
                                       optional<int> end_offset) {
  assert(start_offset.has_value() == end_offset.has_value());

  std::vector<std::string> clang_format_drivers = {
      GetExecutablePathNextToCqueryBinary("cquery-clang-format"),
      "clang-format"};

  for (const std::string& clang_format_driver : clang_format_drivers) {
    std::vector<std::string> args = {clang_format_driver,
                                     "-output-replacements-xml",
                                     "-assume-filename", filename};

    if (start_offset.has_value()) {
      args.push_back("-offset=" + std::to_string(*start_offset));
      args.push_back("-length=" + std::to_string(*end_offset - *start_offset));
    }

    optional<std::string> output = RunExecutable(args, file_contents);
    if (!output || output->empty())
      continue;
    // Do not check if replacements is empty, since that may happen if there are
    // no formatting changes.
    std::vector<Replacement> replacements =
        ParseClangFormatReplacements(*output);
    return ConvertReplacementsToTextEdits(file_contents, replacements);
  }
  return {};
}

TEST_SUITE("clang-format output parsing") {
  TEST_CASE("correct output") {
    // clang-format off
    std::vector<Replacement> replacements = ParseClangFormatReplacements(
      "<?xml version='1.0'?>"
      "<replacements xml:space='preserve' incomplete_format='false'>"
      "<replacement offset='780' length='82'>#include &lt;doctest/doctest.h>&#10;#include &lt;loguru.hpp>&#10;</replacement>"
      "<replacement offset='1448' length='0'> bar foo </replacement>"
      "<replacement offset='1449' length='1'>  </replacement>"
      "<replacement offset='1' length='2'>\t</replacement>"
      "<replacement offset='1' length='2'> \t </replacement>"
      "</replacements>");
    // clang-format on

    auto c = [](Replacement r, int offset, int length,
                const std::string& text) {
      REQUIRE(r.offset == offset);
      REQUIRE(r.length == length);
      REQUIRE(r.text == text);
    };

    REQUIRE(replacements.size() == 5);
    c(replacements[0], 780, 82,
      "#include <doctest/doctest.h>\n#include <loguru.hpp>\n");
    c(replacements[1], 1448, 0, " bar foo ");
    c(replacements[2], 1449, 1, "  ");
    c(replacements[3], 1, 2, "\t");
    c(replacements[4], 1, 2, " \t ");
  }

  TEST_CASE("no replacements") {
    std::vector<Replacement> replacements = ParseClangFormatReplacements(R"(
      <?xml version='1.0'?>
      <replacements xml:space='preserve' incomplete_format='false'>
      </replacements>
    )");
    REQUIRE(replacements.empty());
  }

  TEST_CASE("invalid output") {
    std::vector<Replacement> replacements = ParseClangFormatReplacements(R"(
      <?xml version='1.0'?>
      <reAAAAplacements xml:space='preserve' incomplete_format='false'>
      <replacement offset='780' length='82'>#include &lt;doctest/doctest.h>&#10;#include &lt;loguru.hpp>&#10;</replacement>
      <replacement offset='1448' length='0'>barfoo</replacement>
      <replacement offset='1449' length='1'></replacement>
      </replacements>
    )");
    REQUIRE(replacements.empty());

    replacements = ParseClangFormatReplacements("basd");
    REQUIRE(replacements.empty());

    replacements = ParseClangFormatReplacements("");
    REQUIRE(replacements.empty());
  }
}
