class Foo {
  void foo() {}
};

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
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["1:7-1:10"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 17922201480358737771,
      "short_name": "foo",
      "detailed_name": "void Foo::foo()",
      "kind": 16,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "2:8-2:11",
      "definition_extent": "2:3-2:16",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
*/
