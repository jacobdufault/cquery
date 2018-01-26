void foo();

void usage() {
  foo();
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "1:6-1:9",
          "extent": "1:1-1:11",
          "content": "void foo()",
          "param_spellings": []
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@4:3-4:6"],
      "callees": []
    }, {
      "id": 1,
      "usr": 6767773193109753523,
      "short_name": "usage",
      "detailed_name": "void usage()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:11",
      "definition_extent": "3:1-5:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@4:3-4:6"]
    }],
  "vars": []
}
*/
