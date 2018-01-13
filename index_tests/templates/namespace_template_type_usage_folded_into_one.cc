namespace ns {
  template<typename T>
  class Foo {};

  Foo<int> a;
  Foo<bool> b;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 14042997404480181958,
      "short_name": "Foo",
      "detailed_name": "ns::Foo",
      "kind": 7,
      "definition_spelling": "3:9-3:12",
      "definition_extent": "3:3-3:15",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": ["3:9-3:12", "5:3-5:6", "6:3-6:6"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 15768138241775955040,
      "short_name": "a",
      "detailed_name": "Foo<int> ns::a",
      "definition_spelling": "5:12-5:13",
      "definition_extent": "5:3-5:13",
      "variable_type": 0,
      "kind": 13,
      "uses": ["5:12-5:13"]
    }, {
      "id": 1,
      "usr": 3182917058194750998,
      "short_name": "b",
      "detailed_name": "Foo<bool> ns::b",
      "definition_spelling": "6:13-6:14",
      "definition_extent": "6:3-6:14",
      "variable_type": 0,
      "kind": 13,
      "uses": ["6:13-6:14"]
    }]
}
*/
