void foo(int a) {
  a += 10;
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
      "usr": 11998306017310352355,
      "short_name": "foo",
      "detailed_name": "void foo(int a)",
      "kind": 12,
      "storage": 1,
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
      "declarations": [],
      "definition_spelling": "1:14-1:15",
      "definition_extent": "1:10-1:15",
      "variable_type": 0,
      "uses": ["2:3-2:4"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }]
}
*/
