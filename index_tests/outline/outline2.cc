//#pragma once

#include <string>
#include <vector>

struct CompilationEntry {
  std::string directory;
  std::string filename;
  std::vector<std::string> args;
};

std::vector<CompilationEntry> LoadCompilationEntriesFromDirectory(const std::string& project_directory);

/*
TEXT_REPLACE:
std::__cxx11::string <===> std::string
c:@N@std@string <===> c:@N@std@N@__1@T@string
c:@N@std@T@string <===> c:@N@std@N@__1@T@string
c:@N@std@N@__cxx11@T@string <===> c:@N@std@N@__1@T@string
c:@N@std@ST>2#T#T@vector <===> c:@N@std@N@__1@ST>2#T#T@vector
c:@F@LoadCompilationEntriesFromDirectory#&1$@N@std@N@__1@S@basic_string>#C#$@N@std@N@__1@S@char_traits>#C#$@N@std@N@__1@S@allocator>#C# <===> c:@F@LoadCompilationEntriesFromDirectory#&1$@N@std@S@basic_string>#C#$@N@std@S@char_traits>#C#$@N@std@S@allocator>#C#
c:@F@LoadCompilationEntriesFromDirectory#&1$@N@std@N@__cxx11@S@basic_string>#C#$@N@std@S@char_traits>#C#$@N@std@S@allocator>#C# <===> c:@F@LoadCompilationEntriesFromDirectory#&1$@N@std@S@basic_string>#C#$@N@std@S@char_traits>#C#$@N@std@S@allocator>#C#
4160338041907786 <===> 14151982074805896770
7543170857910783654 <===> 14151982074805896770
9802818309312685221 <===> 11244864715202245734
7636646237071509980 <===> 14151982074805896770
9178760565669096175 <===> 10956461108384510180
10468929532989002392 <===> 11244864715202245734
4160338041907786 <===> 14151982074805896770
9802818309312685221 <===> 11244864715202245734

OUTPUT:
{
  "includes": [{
      "line": 2,
      "resolved_path": "&string"
    }, {
      "line": 3,
      "resolved_path": "&vector"
    }],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 4992269036372211530,
      "short_name": "CompilationEntry",
      "detailed_name": "CompilationEntry",
      "kind": 6,
      "definition_spelling": "6:8-6:24",
      "definition_extent": "6:1-10:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1, 2],
      "instances": [],
      "uses": ["6:8-6:24", "12:13-12:29"]
    }, {
      "id": 1,
      "usr": 14151982074805896770,
      "short_name": "string",
      "detailed_name": "std::string",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": ["7:8-7:14", "8:8-8:14", "9:20-9:26", "12:78-12:84"]
    }, {
      "id": 2,
      "usr": 5401847601697785946,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["7:3-7:6", "8:3-8:6", "9:3-9:6", "9:15-9:18", "12:1-12:4", "12:73-12:76"]
    }, {
      "id": 3,
      "usr": 10956461108384510180,
      "short_name": "vector",
      "detailed_name": "std::vector",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [2],
      "uses": ["9:8-9:14", "12:6-12:12"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 11244864715202245734,
      "short_name": "LoadCompilationEntriesFromDirectory",
      "detailed_name": "std::vector<CompilationEntry> LoadCompilationEntriesFromDirectory(const std::string &project_directory)",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "12:31-12:66",
          "extent": "12:1-12:104",
          "content": "std::vector<CompilationEntry> LoadCompilationEntriesFromDirectory(const std::string& project_directory)",
          "param_spellings": ["12:86-12:103"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 1153224798516629792,
      "short_name": "directory",
      "detailed_name": "std::string CompilationEntry::directory",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "7:15-7:24",
      "definition_extent": "7:3-7:24",
      "variable_type": 1,
      "declaring_type": 0,
      "uses": ["7:15-7:24"]
    }, {
      "id": 1,
      "usr": 2255668374222866345,
      "short_name": "filename",
      "detailed_name": "std::string CompilationEntry::filename",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "8:15-8:23",
      "definition_extent": "8:3-8:23",
      "variable_type": 1,
      "declaring_type": 0,
      "uses": ["8:15-8:23"]
    }, {
      "id": 2,
      "usr": 12616880765274259414,
      "short_name": "args",
      "detailed_name": "std::vector<std::string> CompilationEntry::args",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "9:28-9:32",
      "definition_extent": "9:3-9:32",
      "variable_type": 3,
      "declaring_type": 0,
      "uses": ["9:28-9:32"]
    }]
}
*/
