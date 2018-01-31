template<typename T>
struct Foo {
  static constexpr int var = 3;
};

int a = Foo<int>::var;
int b = Foo<bool>::var;

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
      "definition_extent": "2:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["6:9-6:12", "7:9-7:12"]
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
      "instances": [0, 1, 2],
      "uses": []
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 13545144895171991916,
      "detailed_name": "const int Foo::var",
      "short_name_offset": 15,
      "short_name_size": 3,
      "hover": "const int Foo::var = 3",
      "declarations": ["3:24-3:27"],
      "variable_type": 1,
      "uses": ["6:19-6:22", "7:20-7:23"],
      "parent_id": 18446744073709551615,
      "parent_kind": 0,
      "kind": 21,
      "storage": 3
    }, {
      "id": 1,
      "usr": 16721564935990383768,
      "detailed_name": "int a",
      "short_name_offset": 4,
      "short_name_size": 1,
      "hover": "int a = Foo<int>::var",
      "declarations": [],
      "definition_spelling": "6:5-6:6",
      "definition_extent": "6:1-6:22",
      "variable_type": 1,
      "uses": [],
      "parent_id": 18446744073709551615,
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 2,
      "usr": 12028309045033782423,
      "detailed_name": "int b",
      "short_name_offset": 4,
      "short_name_size": 1,
      "hover": "int b = Foo<bool>::var",
      "declarations": [],
      "definition_spelling": "7:5-7:6",
      "definition_extent": "7:1-7:23",
      "variable_type": 1,
      "uses": [],
      "parent_id": 18446744073709551615,
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }]
}
*/
