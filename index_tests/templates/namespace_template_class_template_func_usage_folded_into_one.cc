namespace ns {
  template<typename T>
  struct Foo {
    template<typename R>
    static int foo() {
      return 3;
    }
  };

  int a = Foo<int>::foo<float>();
  int b = Foo<bool>::foo<double>();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 11072669167287398027,
      "detailed_name": "ns",
      "short_name": "ns",
      "kind": 2,
      "definition_spelling": "1:11-1:13",
      "definition_extent": "1:1-12:2",
      "parents": [1],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": ["1:11-1:13|-1|1|4"]
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
      "usr": 14042997404480181958,
      "detailed_name": "ns::Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "3:10-3:13",
      "definition_extent": "3:3-8:4",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["10:11-10:14|-1|1|4", "11:11-11:14|-1|1|4"]
    }, {
      "id": 3,
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
      "usr": 8221803074608342407,
      "detailed_name": "int ns::Foo::foo()",
      "short_name": "foo",
      "kind": 18,
      "storage": 3,
      "declarations": [],
      "definition_spelling": "5:16-5:19",
      "definition_extent": "5:5-7:6",
      "declaring_type": 2,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["10:21-10:24|0|2|64", "11:22-11:25|0|2|64"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 15768138241775955040,
      "detailed_name": "int ns::a",
      "short_name": "a",
      "hover": "int ns::a = Foo<int>::foo<float>()",
      "declarations": [],
      "definition_spelling": "10:7-10:8",
      "definition_extent": "10:3-10:33",
      "variable_type": 3,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 3182917058194750998,
      "detailed_name": "int ns::b",
      "short_name": "b",
      "hover": "int ns::b = Foo<bool>::foo<double>()",
      "declarations": [],
      "definition_spelling": "11:7-11:8",
      "definition_extent": "11:3-11:35",
      "variable_type": 3,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 13,
      "storage": 1
    }]
}
*/
