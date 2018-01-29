class Foo {};
void f() {
  auto x = new Foo();
  auto* y = new Foo();
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
      "kind": 7,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-1:13",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": ["3:16-3:19", "4:17-4:20"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 880549676430489861,
      "short_name": "f",
      "detailed_name": "void f()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "2:6-2:7",
      "definition_extent": "2:1-5:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 9275666070987716270,
      "short_name": "x",
      "detailed_name": "Foo *x",
      "declarations": [],
      "definition_spelling": "3:8-3:9",
      "definition_extent": "3:3-3:21",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 16202433437488621027,
      "short_name": "y",
      "detailed_name": "Foo *y",
      "declarations": [],
      "definition_spelling": "4:9-4:10",
      "definition_extent": "4:3-4:22",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
