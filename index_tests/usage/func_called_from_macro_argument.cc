#define MACRO_CALL(e) e

bool called(bool a, bool b);

void caller() {
  MACRO_CALL(called(true, true));
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "usr": 3787803219955606747,
      "short_name": "called",
      "detailed_name": "bool called(bool a, bool b)",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "3:6-3:12",
          "extent": "3:1-3:28",
          "content": "bool called(bool a, bool b)",
          "param_spellings": ["3:18-3:19", "3:26-3:27"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@6:14-6:20"],
      "callees": []
    }, {
      "id": 1,
      "usr": 11404881820527069090,
      "short_name": "caller",
      "detailed_name": "void caller()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:6-5:12",
      "definition_extent": "5:1-7:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@6:14-6:20"]
    }],
  "vars": [{
      "id": 0,
      "usr": 1290746656694198202,
      "short_name": "MACRO_CALL",
      "detailed_name": "MACRO_CALL",
      "kind": 4,
      "storage": 0,
      "hover": "#define MACRO_CALL(e) e",
      "declarations": [],
      "definition_spelling": "1:9-1:19",
      "definition_extent": "1:9-1:24",
      "uses": ["6:3-6:13"]
    }]
}
*/