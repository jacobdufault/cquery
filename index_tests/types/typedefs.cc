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
      "short_name": "func",
      "kind": 252,
      "hover": "typedef int (func)(const int *a, const int *b)",
      "declarations": [],
      "spell": "1:14-1:18|-1|1|2",
      "extent": "1:1-1:47|-1|1|0",
      "bases": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:14-1:18|-1|1|4", "2:8-2:12|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 8105378401105136463,
      "detailed_name": "func g",
      "short_name": "g",
      "kind": 12,
      "storage": 3,
      "declarations": [{
          "spell": "2:13-2:14|-1|1|1",
          "param_spellings": ["2:13-2:13", "2:13-2:13"]
        }],
      "bases": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/