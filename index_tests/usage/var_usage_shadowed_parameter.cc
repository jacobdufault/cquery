void foo(int a) {
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
      "short_name_offset": 0,
      "short_name_size": 0,
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
      "usr": 11998306017310352355,
      "detailed_name": "void foo(int a)",
      "short_name_offset": 5,
      "short_name_size": 3,
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:9",
      "definition_extent": "1:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 11608231465452906059,
      "detailed_name": "int a",
      "short_name_offset": 4,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "1:14-1:15",
      "definition_extent": "1:10-1:15",
      "variable_type": 0,
      "uses": ["2:3-2:4", "7:3-7:4"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }, {
      "id": 1,
      "usr": 8011559936501990179,
      "detailed_name": "int a",
      "short_name_offset": 4,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "4:9-4:10",
      "definition_extent": "4:5-4:10",
      "variable_type": 0,
      "uses": ["5:5-5:6"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
