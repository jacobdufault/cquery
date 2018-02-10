class Foo {
  Foo* make();
};

Foo* Foo::make() {
  Foo f;
  return nullptr;
}

extern Foo foo;

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [0, 1],
      "uses": ["2:3-2:6|-1|1|4", "5:1-5:4|-1|1|4", "5:6-5:9|-1|1|4", "6:3-6:6|-1|1|4", "10:8-10:11|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 9488177941273031343,
      "detailed_name": "Foo *Foo::make()",
      "short_name": "make",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "2:8-2:12",
          "extent": "2:3-2:14",
          "content": "Foo* make()",
          "param_spellings": []
        }],
      "definition_spelling": "5:11-5:15",
      "definition_extent": "5:1-8:2",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 14873619387499024780,
      "detailed_name": "Foo f",
      "short_name": "f",
      "declarations": [],
      "definition_spelling": "6:7-6:8",
      "definition_extent": "6:3-6:8",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 14455976355866885943,
      "detailed_name": "Foo foo",
      "short_name": "foo",
      "declarations": ["10:12-10:15"],
      "variable_type": 0,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 2
    }]
}
*/
