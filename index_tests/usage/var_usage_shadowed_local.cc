void foo() {
  int a;
  a = 1;
  {
    int a;
    a = 2;
  }
  a = 3;
}
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:9",
      "definition_extent": "1:1-9:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 17941402366659878910,
      "detailed_name": "int a",
      "short_name": "a",
      "declarations": [],
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "variable_type": 0,
      "uses": ["3:3-3:4|0|3|4", "8:3-8:4|0|3|4"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 11094102496276744608,
      "detailed_name": "int a",
      "short_name": "a",
      "declarations": [],
      "definition_spelling": "5:9-5:10",
      "definition_extent": "5:5-5:10",
      "variable_type": 0,
      "uses": ["6:5-6:6|0|3|4"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
