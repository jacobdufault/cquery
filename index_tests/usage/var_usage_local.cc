void foo() {
  int x;
  x = 3;
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name_offset": 5,
      "short_name_size": 3,
      "kind": 12,
      "storage": 1,
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
      "detailed_name": "int x",
      "short_name_offset": 4,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "variable_type": 0,
      "uses": ["3:3-3:4"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
