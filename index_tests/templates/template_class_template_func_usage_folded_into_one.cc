template<typename T>
struct Foo {
  template<typename R>
  static int foo() {
    return 3;
  }
};

int a = Foo<int>::foo<float>();
int b = Foo<bool>::foo<double>();

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 10528472276654770367,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "2:8-2:11",
      "definition_extent": "2:1-7:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["9:9-9:12|-1|1|4", "10:9-10:12|-1|1|4"]
    }, {
      "id": 1,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
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
      "usr": 9034026360701857235,
      "detailed_name": "int Foo::foo()",
      "short_name": "foo",
      "kind": 18,
      "storage": 3,
      "declarations": [],
      "definition_spelling": "4:14-4:17",
      "definition_extent": "4:3-6:4",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["9:19-9:22|-1|1|64", "10:20-10:23|-1|1|64"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 16721564935990383768,
      "detailed_name": "int a",
      "short_name": "a",
      "hover": "int a = Foo<int>::foo<float>()",
      "declarations": [],
      "definition_spelling": "9:5-9:6",
      "definition_extent": "9:1-9:31",
      "variable_type": 1,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 12028309045033782423,
      "detailed_name": "int b",
      "short_name": "b",
      "hover": "int b = Foo<bool>::foo<double>()",
      "declarations": [],
      "definition_spelling": "10:5-10:6",
      "definition_extent": "10:1-10:33",
      "variable_type": 1,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }]
}
*/
