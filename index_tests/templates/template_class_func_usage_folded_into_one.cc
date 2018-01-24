template<typename T>
struct Foo {
  static int foo() {
    return 3;
  }
};

int a = Foo<int>::foo();
int b = Foo<bool>::foo();

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 10528472276654770367,
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 7,
      "definition_spelling": "2:8-2:11",
      "definition_extent": "2:1-6:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["2:8-2:11", "8:9-8:12", "9:9-9:12"]
    }, {
      "id": 1,
      "usr": 17,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 8340731781048851399,
      "short_name": "foo",
      "detailed_name": "int Foo::foo()",
      "kind": 18,
      "declarations": [],
      "definition_spelling": "3:14-3:17",
      "definition_extent": "3:3-5:4",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["-1@8:19-8:22", "-1@9:20-9:23"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 16721564935990383768,
      "short_name": "a",
      "detailed_name": "int a",
      "hover": "int a = Foo<int>::foo()",
      "definition_spelling": "8:5-8:6",
      "definition_extent": "8:1-8:24",
      "variable_type": 1,
      "kind": 13,
      "uses": ["8:5-8:6"]
    }, {
      "id": 1,
      "usr": 12028309045033782423,
      "short_name": "b",
      "detailed_name": "int b",
      "hover": "int b = Foo<bool>::foo()",
      "definition_spelling": "9:5-9:6",
      "definition_extent": "9:1-9:25",
      "variable_type": 1,
      "kind": 13,
      "uses": ["9:5-9:6"]
    }]
}
*/
