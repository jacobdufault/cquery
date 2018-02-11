namespace hello {
class Foo {
  void foo() {}
};
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
      "spell": "1:11-1:16|-1|1|2",
      "extent": "1:1-5:2|-1|1|0",
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
      "spell": "2:7-2:10|0|2|2",
      "extent": "2:1-4:2|0|2|2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 10487325150128053272,
      "detailed_name": "void hello::Foo::foo()",
      "short_name": "foo",
      "kind": 16,
      "storage": 1,
      "declarations": [],
      "spell": "3:8-3:11|2|2|2",
      "extent": "3:3-3:16|2|2|2",
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
