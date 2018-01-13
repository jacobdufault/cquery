void foo(int p) {
  { int p = 0; }
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 11998306017310352355,
      "short_name": "foo",
      "detailed_name": "void foo(int p)",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "1:6-1:9",
      "definition_extent": "1:1-3:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 5875271969926422921,
      "short_name": "p",
      "detailed_name": "int p",
      "definition_spelling": "1:14-1:15",
      "definition_extent": "1:10-1:15",
      "kind": 25,
      "uses": ["1:14-1:15"]
    }, {
      "id": 1,
      "usr": 2147918703972955240,
      "short_name": "p",
      "detailed_name": "int p",
      "definition_spelling": "2:9-2:10",
      "definition_extent": "2:5-2:14",
      "kind": 13,
      "uses": ["2:9-2:10"]
    }]
}
*/
