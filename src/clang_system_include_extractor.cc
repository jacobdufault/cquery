#include "clang_system_include_extractor.h"

#include "compiler.h"
#include "platform.h"
#include "utils.h"

#include <doctest/doctest.h>
#include <loguru/loguru.hpp>

namespace {
std::vector<std::string> ExtractSystemIncludePaths(
    const std::string& clang_output) {
  //
  // Parse the output of ie, `clang++ -E -xc++ - -v < nul`. We are looking for a
  // section like this:
  //
  // ...
  // #include "..." search starts here:
  // #include <...> search starts here:
  //  C:\Program Files\LLVM\lib\clang\6.0.0\include
  //  C:\Program Files (x86)\Microsoft Visual
  //  Studio\2017\Community\VC\Tools\MSVC\14.12.25827\include C:\Program Files
  //  (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt C:\Program Files
  //  (x86)\Windows Kits\10\include\10.0.15063.0\shared C:\Program Files
  //  (x86)\Windows Kits\10\include\10.0.15063.0\um C:\Program Files
  //  (x86)\Windows Kits\10\include\10.0.15063.0\winrt C:\Foobar (framework
  //  directory)
  // End of search list.
  // ...
  //

  // On macOS, clang will add ` (framework directory)` to some of the lines.
  // That should be removed.
  constexpr const char kIncludePostfixToRemove[] = " (framework directory)";

  std::vector<std::string> lines = SplitString(clang_output, "\n");
  std::vector<std::string> output;
  bool in_system_include_search_section = false;
  for (auto& line : lines) {
    TrimInPlace(line);
    if (!in_system_include_search_section) {
      in_system_include_search_section =
          line == "#include <...> search starts here:";
      continue;
    }
    if (line == "End of search list.") {
      in_system_include_search_section = false;
      break;
    }
    if (EndsWith(line, kIncludePostfixToRemove)) {
      line = line.substr(0, line.size() - sizeof(kIncludePostfixToRemove) + 1);
      output.push_back("-iframework" + line);
    } else {
      output.push_back("-isystem" + line);
    }
  }

  // We did not see an "End of search list.", so don't return data.
  if (in_system_include_search_section) {
    LOG_S(WARNING) << "Unable to parse clang output\n" << clang_output;
    return {};
  }

  return output;
}

}  // namespace

// Run clang specified by `clang_binary` and return the set of system includes
// it uses.
std::vector<std::string> FindSystemIncludeDirectories(
    const std::vector<std::string>& compiler_drivers,
    const std::string& language,
    const std::string& working_directory,
    const std::vector<std::string>& extra_flags) {
  LOG_S(INFO) << "Using compiler drivers " << StringJoin(compiler_drivers);
  for (const std::string& compiler_driver : compiler_drivers) {
    std::vector<std::string> flags = {
        compiler_driver, "-E", "-x", language, "-", "-v",
    };

    CompilerType compiler_type = FindCompilerType(compiler_driver);
    CompilerAppendsFlagIfAccept(
        compiler_type, "-working-directory=" + working_directory, flags);

    AddRange(&flags, extra_flags);

    LOG_S(INFO) << "Running " << StringJoin(flags, " ");
    optional<std::string> clang_output = RunExecutable(flags, "");
    if (!clang_output || clang_output->empty())
      continue;

    LOG_S(INFO) << "Output:\n" << Trim(*clang_output);
    std::vector<std::string> paths = ExtractSystemIncludePaths(*clang_output);
    if (!paths.empty())
      return paths;
  }
  return {};
}

TEST_SUITE("System include extraction") {
  TEST_CASE("clang system include directory extraction") {
    std::vector<std::string> paths = ExtractSystemIncludePaths(
        "asdfasdf\n"
        "   #include <...> search starts here:  \n"
        "one\n"
        "  two\r\n"
        "three \r\n"
        "four\n"
        "  five (framework directory)  \n"
        "six\n"
        "End of search list.\n"
        "asdfasdf");
    REQUIRE(paths == std::vector<std::string>{
                         "-isystemone",
                         "-isystemtwo",
                         "-isystemthree",
                         "-isystemfour",
                         "-iframeworkfive",
                         "-isystemsix",
                     });

    // Same thing, but do not have a proper end of search list.
    paths = ExtractSystemIncludePaths(
        "asdfasdf\n"
        "   #include <...> search starts here:  \n"
        "one\n"
        "Enasdfd of search list.\n"
        "asdfasdf");
    REQUIRE(paths == std::vector<std::string>{});

    // Empty input
    paths = ExtractSystemIncludePaths("");
    REQUIRE(paths == std::vector<std::string>{});

    // Input without a include search section.
    paths = ExtractSystemIncludePaths(
        "foobar1\n"
        "End of search list.\n"
        "foobar2");
    REQUIRE(paths == std::vector<std::string>{});
  }
}
