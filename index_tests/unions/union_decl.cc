union Foo {
  int a;
  bool b;
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 8501689086387244262,
      "detailed_name": "Foo",
      "short_name_offset": 0,
      "short_name_size": 3,
      "kind": 10,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": []
    }, {
      "id": 1,
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
      "instances": [0],
      "uses": []
    }, {
      "id": 2,
      "usr": 3,
      "detailed_name": "",
      "short_name_offset": 0,
      "short_name_size": 0,
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [1],
      "uses": []
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 9529311430721959843,
      "detailed_name": "int Foo::a",
      "short_name_offset": 9,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "variable_type": 1,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 1,
      "usr": 8804696910588009104,
      "detailed_name": "bool Foo::b",
      "short_name_offset": 0,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "3:8-3:9",
      "definition_extent": "3:3-3:9",
      "variable_type": 2,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }]
}
*/
