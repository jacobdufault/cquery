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
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 6,
      "definition_spelling": "4:8-4:11",
      "definition_extent": "4:1-6:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["4:8-4:11", "5:12-5:15"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 13788753348312146871,
      "short_name": "Foo",
      "detailed_name": "void Foo::Foo(Foo &&)",
      "kind": 22,
      "declarations": [],
      "definition_spelling": "5:12-5:15",
      "definition_extent": "5:12-5:16",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 10677751717622394455,
      "short_name": "x",
      "detailed_name": "int x",
      "hover": "int x = A",
      "definition_spelling": "8:5-8:6",
      "definition_extent": "8:1-8:10",
      "kind": 13,
      "uses": ["8:5-8:6"]
    }, {
      "id": 1,
      "usr": 7651988378939587454,
      "short_name": "A",
      "detailed_name": "A",
      "hover": "#define A 5",
      "definition_spelling": "1:9-1:10",
      "definition_extent": "1:9-1:12",
      "kind": 4,
      "uses": ["1:9-1:10", "8:9-8:10"]
    }, {
      "id": 2,
      "usr": 14946041066794678724,
      "short_name": "DISALLOW",
      "detailed_name": "DISALLOW",
      "hover": "#define DISALLOW(type) type(type&&) = delete;",
      "definition_spelling": "2:9-2:17",
      "definition_extent": "2:9-2:46",
      "kind": 4,
      "uses": ["2:9-2:17", "5:3-5:11"]
    }]
}
*/
