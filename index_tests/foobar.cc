enum A {};
enum B {};

template<typename T>
struct Foo {
  struct Inner {};
};

Foo<A>::Inner a;
Foo<B> b;
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 6697181287623958829,
      "detailed_name": "A",
      "short_name": "A",
      "kind": 5,
      "definition_spelling": "1:6-1:7",
      "definition_extent": "1:1-1:10",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["9:5-9:6|-1|1|4"]
    }, {
      "id": 1,
      "usr": 13892793056005362145,
      "detailed_name": "B",
      "short_name": "B",
      "kind": 5,
      "definition_spelling": "2:6-2:7",
      "definition_extent": "2:1-2:10",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["10:5-10:6|-1|1|4"]
    }, {
      "id": 2,
      "usr": 10528472276654770367,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "5:8-5:11",
      "definition_extent": "5:1-7:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [1],
      "uses": ["9:1-9:4|-1|1|4", "10:1-10:4|-1|1|4"]
    }, {
      "id": 3,
      "usr": 13938528237873543349,
      "detailed_name": "Foo::Inner",
      "short_name": "Inner",
      "kind": 6,
      "definition_spelling": "6:10-6:15",
      "definition_extent": "6:3-6:18",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["9:9-9:14|-1|1|4"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 16721564935990383768,
      "detailed_name": "Foo<A>::Inner a",
      "short_name": "a",
      "declarations": [],
      "definition_spelling": "9:15-9:16",
      "definition_extent": "9:1-9:16",
      "variable_type": 3,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 12028309045033782423,
      "detailed_name": "Foo<B> b",
      "short_name": "b",
      "declarations": [],
      "definition_spelling": "10:8-10:9",
      "definition_extent": "10:1-10:9",
      "variable_type": 2,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }]
}
*/
