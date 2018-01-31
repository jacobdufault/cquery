typedef int (func)(const int *a, const int *b);
static func	g;

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 10383876566159302459,
      "detailed_name": "func",
      "short_name_offset": 0,
      "short_name_size": 4,
      "kind": 11,
      "hover": "typedef int (func)(const int *a, const int *b)",
      "definition_spelling": "1:14-1:18",
      "definition_extent": "1:1-1:47",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:14-1:18", "2:8-2:12"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 8105378401105136463,
      "detailed_name": "func g",
      "short_name_offset": 5,
      "short_name_size": 1,
      "kind": 12,
      "storage": 3,
      "declarations": [{
          "spelling": "2:13-2:14",
          "extent": "2:1-2:14",
          "content": "static func g",
          "param_spellings": ["2:13-2:13", "2:13-2:13"]
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