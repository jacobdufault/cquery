union Foo {
  int a : 5;
  bool b : 3;
};

Foo f;

void act(Foo*) {
  f.a = 3;
}

/*
// TODO: instantiations on Foo should include parameter?

OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 8501689086387244262,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 10,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [2],
      "uses": ["6:1-6:4|-1|1|4", "8:10-8:13|-1|1|4"]
    }, {
      "id": 1,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
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
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [1],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 13982179977217945200,
      "detailed_name": "void act(Foo *)",
      "short_name": "act",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "8:6-8:9",
      "definition_extent": "8:1-10:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 9529311430721959843,
      "detailed_name": "int Foo::a",
      "short_name": "a",
      "hover": "int Foo::a : 5",
      "declarations": [],
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:12",
      "variable_type": 1,
      "uses": ["9:5-9:6|0|3|4"],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 1,
      "usr": 8804696910588009104,
      "detailed_name": "bool Foo::b",
      "short_name": "b",
      "hover": "bool Foo::b : 3",
      "declarations": [],
      "definition_spelling": "3:8-3:9",
      "definition_extent": "3:3-3:13",
      "variable_type": 2,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 2,
      "usr": 2933643612409209903,
      "detailed_name": "Foo f",
      "short_name": "f",
      "declarations": [],
      "definition_spelling": "6:5-6:6",
      "definition_extent": "6:1-6:6",
      "variable_type": 0,
      "uses": ["9:3-9:4|0|3|4"],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }]
}
*/
