void foo() {
  int x;
  x = 3;
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
      "usr": 4259594751088586730,
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "1:6-1:9",
      "definition_extent": "1:1-4:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 8534460107894911680,
      "short_name": "x",
      "detailed_name": "int x",
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "kind": 13,
      "uses": ["2:7-2:8", "3:3-3:4"]
    }]
}
*/
