enum Foo : int {
  A,
  B = 20
};

typedef int int32_t;

enum class E : int32_t {
  E0,
  E20 = 20
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 16985894625255407295,
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 5,
      "definition_spelling": "1:6-1:9",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": ["1:6-1:9"]
    }, {
      "id": 1,
      "usr": 14939241684006947339,
      "short_name": "int32_t",
      "detailed_name": "int32_t",
      "kind": 11,
      "hover": "typedef int int32_t",
      "definition_spelling": "6:13-6:20",
      "definition_extent": "6:1-6:20",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["6:13-6:20", "8:12-8:13"]
    }, {
      "id": 2,
      "usr": 2986879766914123941,
      "short_name": "E",
      "detailed_name": "E",
      "kind": 5,
      "definition_spelling": "8:12-8:13",
      "definition_extent": "8:1-11:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [2, 3],
      "instances": [],
      "uses": ["8:12-8:13"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 439339022761937396,
      "short_name": "A",
      "detailed_name": "Foo::A",
      "kind": 15,
      "storage": 0,
      "hover": "Foo::A = 0",
      "definition_spelling": "2:3-2:4",
      "definition_extent": "2:3-2:4",
      "variable_type": 0,
      "declaring_type": 0,
      "uses": []
    }, {
      "id": 1,
      "usr": 15962370213938840720,
      "short_name": "B",
      "detailed_name": "Foo::B",
      "kind": 15,
      "storage": 0,
      "hover": "Foo::B = 20",
      "definition_spelling": "3:3-3:4",
      "definition_extent": "3:3-3:9",
      "variable_type": 0,
      "declaring_type": 0,
      "uses": []
    }, {
      "id": 2,
      "usr": 16614320383091394267,
      "short_name": "E0",
      "detailed_name": "E::E0",
      "kind": 15,
      "storage": 0,
      "hover": "E::E0 = 0",
      "definition_spelling": "9:3-9:5",
      "definition_extent": "9:3-9:5",
      "variable_type": 2,
      "declaring_type": 2,
      "uses": []
    }, {
      "id": 3,
      "usr": 16847439761518576294,
      "short_name": "E20",
      "detailed_name": "E::E20",
      "kind": 15,
      "storage": 0,
      "hover": "E::E20 = 20",
      "definition_spelling": "10:3-10:6",
      "definition_extent": "10:3-10:11",
      "variable_type": 2,
      "declaring_type": 2,
      "uses": []
    }]
}
*/
