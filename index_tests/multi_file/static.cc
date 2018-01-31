#include "static.h"

void Buffer::CreateSharedBuffer() {}

/*
OUTPUT: static.h
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 9411323049603567600,
      "short_name": "Buffer",
      "detailed_name": "Buffer",
      "kind": 6,
      "definition_spelling": "3:8-3:14",
      "definition_extent": "3:1-5:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 14576076421851654759,
      "detailed_name": "void Buffer::CreateSharedBuffer()",
      "short_name_offset": 13,
      "short_name_size": 18,
      "kind": 18,
      "storage": 3,
      "declarations": [{
          "spelling": "4:15-4:33",
          "extent": "4:3-4:35",
          "content": "static void CreateSharedBuffer()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
OUTPUT: static.cc
{
  "includes": [{
      "line": 0,
      "resolved_path": "&static.h"
    }],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 9411323049603567600,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["3:6-3:12"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 14576076421851654759,
      "detailed_name": "void Buffer::CreateSharedBuffer()",
      "short_name_offset": 13,
      "short_name_size": 18,
      "kind": 18,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:14-3:32",
      "definition_extent": "3:1-3:37",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
*/