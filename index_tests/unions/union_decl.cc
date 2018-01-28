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
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 10,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": ["1:7-1:10"]
    }, {
      "id": 1,
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
    }, {
      "id": 2,
      "usr": 3,
      "short_name": "",
      "detailed_name": "",
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
      "short_name": "a",
      "detailed_name": "int Foo::a",
      "kind": 14,
      "storage": 0,
      "declarations": [],
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "variable_type": 1,
      "uses": []
    }, {
      "id": 1,
      "usr": 8804696910588009104,
      "short_name": "b",
      "detailed_name": "bool Foo::b",
      "kind": 14,
      "storage": 0,
      "declarations": [],
      "definition_spelling": "3:8-3:9",
      "definition_extent": "3:3-3:9",
      "variable_type": 2,
      "uses": []
    }]
}
*/
