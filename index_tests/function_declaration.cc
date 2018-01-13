void foo(int a, int b);

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 2747674671862363334,
      "short_name": "foo",
      "detailed_name": "void foo(int a, int b)",
      "kind": 12,
      "declarations": [{
          "spelling": "1:6-1:9",
          "extent": "1:1-1:23",
          "content": "void foo(int a, int b)",
          "param_spellings": ["1:14-1:15", "1:21-1:22"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
*/
