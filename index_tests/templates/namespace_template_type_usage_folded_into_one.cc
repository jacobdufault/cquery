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
      "usr": 11072669167287398027,
      "detailed_name": "ns",
      "short_name": "ns",
      "kind": 2,
      "definition_spelling": "1:11-1:13",
      "definition_extent": "1:1-7:2",
      "parents": [1],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": ["1:11-1:13|-1|1|4"]
    }, {
      "id": 1,
      "usr": 13838176792705659279,
      "detailed_name": "<fundamental>",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [0],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": []
    }, {
      "id": 2,
      "usr": 14042997404480181958,
      "detailed_name": "ns::Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "3:9-3:12",
      "definition_extent": "3:3-3:15",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": ["5:3-5:6|-1|1|4", "6:3-6:6|-1|1|4"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 15768138241775955040,
      "detailed_name": "Foo<int> ns::a",
      "short_name": "a",
      "declarations": [],
      "definition_spelling": "5:12-5:13",
      "definition_extent": "5:3-5:13",
      "variable_type": 2,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 3182917058194750998,
      "detailed_name": "Foo<bool> ns::b",
      "short_name": "b",
      "declarations": [],
      "definition_spelling": "6:13-6:14",
      "definition_extent": "6:3-6:14",
      "variable_type": 2,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 13,
      "storage": 1
    }]
}
*/
