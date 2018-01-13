void foo(int a) {
  a += 10;
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
      "detailed_name": "void foo(int a)",
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
      "usr": 10063793875496522529,
      "short_name": "a",
      "detailed_name": "int a",
      "definition_spelling": "1:14-1:15",
      "definition_extent": "1:10-1:15",
      "kind": 25,
      "uses": ["1:14-1:15", "2:3-2:4"]
    }]
}
*/
