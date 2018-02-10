#define A 5
#define DISALLOW(type) type(type&&) = delete;

struct Foo {
  DISALLOW(Foo);
};

int x = A;

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 6,
      "definition_spelling": "4:8-4:11",
      "definition_extent": "4:1-6:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["5:12-5:15|0|3|4"]
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
      "instances": [0],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 13788753348312146871,
      "detailed_name": "void Foo::Foo(Foo &&)",
      "short_name": "Foo",
      "kind": 22,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:12-5:15",
      "definition_extent": "5:12-5:16",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 10677751717622394455,
      "detailed_name": "int x",
      "short_name": "x",
      "hover": "int x = A",
      "declarations": [],
      "definition_spelling": "8:5-8:6",
      "definition_extent": "8:1-8:10",
      "variable_type": 1,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 7651988378939587454,
      "detailed_name": "A",
      "short_name": "A",
      "hover": "#define A 5",
      "declarations": [],
      "definition_spelling": "1:9-1:10",
      "definition_extent": "1:9-1:12",
      "uses": ["8:9-8:10|-1|1|4"],
      "parent_kind": 0,
      "kind": 4,
      "storage": 0
    }, {
      "id": 2,
      "usr": 14946041066794678724,
      "detailed_name": "DISALLOW",
      "short_name": "DISALLOW",
      "hover": "#define DISALLOW(type) type(type&&) = delete;",
      "declarations": [],
      "definition_spelling": "2:9-2:17",
      "definition_extent": "2:9-2:46",
      "uses": ["5:3-5:11|-1|1|4"],
      "parent_kind": 0,
      "kind": 4,
      "storage": 0
    }]
}
*/
