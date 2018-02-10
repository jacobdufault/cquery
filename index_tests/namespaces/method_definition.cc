namespace hello {
class Foo {
  void foo();
};

void Foo::foo() {}
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 2029211996748007610,
      "detailed_name": "hello",
      "short_name": "hello",
      "kind": 2,
      "definition_spelling": "1:11-1:16",
      "definition_extent": "1:1-7:2",
      "parents": [1],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:11-1:16|-1|1|4"]
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
      "usr": 4508214972876735896,
      "detailed_name": "hello::Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "2:7-2:10",
      "definition_extent": "2:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["6:6-6:9|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 10487325150128053272,
      "detailed_name": "void hello::Foo::foo()",
      "short_name": "foo",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "3:8-3:11",
          "extent": "3:3-3:13",
          "content": "void foo()",
          "param_spellings": []
        }],
      "definition_spelling": "6:11-6:14",
      "definition_extent": "6:1-6:19",
      "declaring_type": 2,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
