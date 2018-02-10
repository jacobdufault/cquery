void called();

template <typename T>
void caller() {
  called();
}

void foo() {
  caller<int>();
}

/*
// NOTE: without caller<int>() instantation caller() is never visited so
// called() is never referenced.
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "usr": 468307235068920063,
      "detailed_name": "void called()",
      "short_name": "called",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "1:6-1:12",
          "extent": "1:1-1:14",
          "content": "void called()",
          "param_spellings": []
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["5:3-5:9|1|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 10177235824697315808,
      "detailed_name": "void caller()",
      "short_name": "caller",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "4:6-4:12",
      "definition_extent": "4:1-6:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["9:3-9:9|2|3|64"],
      "callees": ["5:3-5:9|0|3|64"]
    }, {
      "id": 2,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "8:6-8:9",
      "definition_extent": "8:1-10:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["9:3-9:9|1|3|64"]
    }],
  "vars": []
}
*/