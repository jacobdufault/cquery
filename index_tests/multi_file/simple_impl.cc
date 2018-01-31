#include "simple_header.h"

void impl() {
  header();
}

/*
OUTPUT: simple_header.h
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "usr": 16236105532929924676,
      "detailed_name": "void header()",
      "short_name_offset": 5,
      "short_name_size": 6,
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "3:6-3:12",
          "extent": "3:1-3:14",
          "content": "void header()",
          "param_spellings": []
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
OUTPUT: simple_impl.cc
{
  "includes": [{
      "line": 0,
      "resolved_path": "&simple_header.h"
    }],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "usr": 3373269392705484958,
      "detailed_name": "void impl()",
      "short_name_offset": 5,
      "short_name_size": 4,
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:10",
      "definition_extent": "3:1-5:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["1@4:3-4:9"]
    }, {
      "id": 1,
      "usr": 16236105532929924676,
      "detailed_name": "",
      "short_name_offset": 8,
      "short_name_size": 0,
      "kind": 0,
      "storage": 0,
      "declarations": [],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["0@4:3-4:9"],
      "callees": []
    }],
  "vars": []
}
*/
