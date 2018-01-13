namespace hello {
class Foo {
  void foo();
};
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 4508214972876735896,
      "short_name": "Foo",
      "detailed_name": "hello::Foo",
      "kind": 7,
      "definition_spelling": "2:7-2:10",
      "definition_extent": "2:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["2:7-2:10"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 10487325150128053272,
      "short_name": "foo",
      "detailed_name": "void hello::Foo::foo()",
      "kind": 16,
      "declarations": [{
          "spelling": "3:8-3:11",
          "extent": "3:3-3:13",
          "content": "void foo()",
          "param_spellings": []
        }],
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
