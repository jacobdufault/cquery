struct Foo {
  void Used();
};

void user() {
  auto x = &Foo::Used;
}


/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 6,
      "definition_spelling": "1:8-1:11",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:11", "6:13-6:16"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 18417145003926999463,
      "short_name": "Used",
      "detailed_name": "void Foo::Used()",
      "kind": 16,
      "declarations": [{
          "spelling": "2:8-2:12",
          "extent": "2:3-2:14",
          "content": "void Used()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@6:18-6:22"],
      "callees": []
    }, {
      "id": 1,
      "is_operator": false,
      "usr": 9376923949268137283,
      "short_name": "user",
      "detailed_name": "void user()",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "5:6-5:10",
      "definition_extent": "5:1-7:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@6:18-6:22"]
    }],
  "vars": [{
      "id": 0,
      "usr": 8436636043513449412,
      "short_name": "x",
      "detailed_name": "void (Foo::*)() x",
      "hover": "void (Foo::*)() x = &Foo::Used",
      "definition_spelling": "6:8-6:9",
      "definition_extent": "6:3-6:22",
      "kind": 13,
      "uses": ["6:8-6:9"]
    }]
}
*/
