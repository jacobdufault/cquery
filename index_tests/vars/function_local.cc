struct Foo;

void foo() {
  Foo* a;
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
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["1:8-1:11", "4:3-4:6"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:9",
      "definition_extent": "3:1-5:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 10782632605670042066,
      "short_name": "a",
      "detailed_name": "Foo *a",
      "declarations": [],
      "definition_spelling": "4:8-4:9",
      "definition_extent": "4:3-4:9",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
