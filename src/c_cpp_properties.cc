#include "c_cpp_properties.h"

#include <fstream>
#include <iostream>
#include <regex>

#include <string_view.h>

#include <doctest/doctest.h>
#include <rapidjson/document.h>

#ifdef _WIN32
#define CURRENT_PLATFORM "Win32"
#elif defined(__APPLE__)
#define CURRENT_PLATFORM "Mac"
#else
#define CURRENT_PLATFORM "Linux"
#endif

namespace {

std::string_view kCurrentPlatform = CURRENT_PLATFORM;

optional<CCppProperties> LoadCCppPropertiesFromStr(
    const std::string_view filecontent,
    const std::string& project_dir) {
  CCppProperties res;
  res.args.push_back("%clang");
  rapidjson::Document document;
  document.Parse(filecontent.data());
  if (!document.IsObject())
    return {};
  auto conf_it = document.FindMember("configurations");
  if (conf_it == document.MemberEnd())
    return {};
  if (!conf_it->value.IsArray())
    return {};
  for (auto& conf : conf_it->value.GetArray()) {
    if (!conf.HasMember("name") || conf["name"].GetString() != kCurrentPlatform)
      continue;
    auto def_it = conf.FindMember("defines");
    if (def_it != conf.MemberEnd() && def_it->value.IsArray()) {
      for (auto& def : def_it->value.GetArray()) {
        res.args.push_back(std::string("-D") + def.GetString());
      }
    }

    auto inc_it = conf.FindMember("includePath");
    if (inc_it != conf.MemberEnd() && inc_it->value.IsArray()) {
      for (auto& inc : inc_it->value.GetArray()) {
        // TODO maybe handle "path/**" ?
        auto incpath = std::regex_replace(
            std::string(inc.GetString()),
            std::regex("\\$\\{workspaceFolder\\}"), project_dir);
        res.args.push_back("-I" + incpath);
      }
    }

    if (res.cStandard.empty() && conf.HasMember("cStandard")) {
      res.cStandard = conf["cStandard"].GetString();
      res.args.push_back("%c -std=" + res.cStandard);
    }
    if (res.cppStandard.empty() && conf.HasMember("cppStandard")) {
      res.cppStandard = conf["cppStandard"].GetString();
      res.args.push_back("%cpp -std=" + res.cppStandard);
    }
  }

  return res;
}

}  // namespace

optional<CCppProperties> LoadCCppProperties(const std::string& json_full_path,
                                            const std::string& project_dir) {
  std::ifstream fc_stream(json_full_path);
  if (!fc_stream)
    return {};
  std::string filecontent{std::istreambuf_iterator<char>(fc_stream),
                          std::istreambuf_iterator<char>()};
  return LoadCCppPropertiesFromStr(filecontent, project_dir);
}

TEST_SUITE("CCppProperties") {
  TEST_CASE("basic") {
    const char* testjson = R"(
      {
        "configurations": [
          {
            "name": ")" CURRENT_PLATFORM R"(",
            "browse": {
                "path": [
                    "${workspaceFolder}"
                ],
                "limitSymbolsToIncludedHeaders": true
            },
            "includePath": [
                "${workspaceFolder}",
                "foo",
                "${workspaceFolder}/bar"
            ],
            "defines": [
              "FOO",
              "BAR=1"
            ],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c11",
            "cppStandard": "c++17",
            "intelliSenseMode": "clang-x64"
          }
        ],
        "version": 4
      })";
    auto res = LoadCCppPropertiesFromStr(testjson, "/proj/");
    REQUIRE(res.has_value());
    CCppProperties& val = res.value();
    REQUIRE_EQ(val.cStandard, "c11");
    REQUIRE_EQ(val.cppStandard, "c++17");
    std::vector<std::string> args{
        "%clang", "-DFOO",        "-DBAR=1",     "-I/proj/",
        "-Ifoo",  "-I/proj//bar", "%c -std=c11", "%cpp -std=c++17"};
    bool args_equal = val.args == args;
    if (!args_equal) {
      if (val.args.size() != args.size()) {
        std::cout << "\tval.args size " << val.args.size() << " , args size "
                  << args.size() << std::endl;
      }
      for (size_t i = 0; i < std::min(val.args.size(), args.size()); ++i) {
        auto& a1 = val.args[i];
        auto& a2 = args[i];
        if (a1 != a2) {
          std::cout << "\tArg " << a1 << " != " << a2 << std::endl;
        }
      }
    }
    REQUIRE(args_equal);
  }
}
