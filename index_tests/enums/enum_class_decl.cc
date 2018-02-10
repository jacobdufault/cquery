typedef unsigned char uint8_t;
enum class Foo : uint8_t {
  A,
  B = 20
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 2010430204259339553,
      "detailed_name": "uint8_t",
      "short_name": "uint8_t",
      "kind": 11,
      "hover": "typedef unsigned char uint8_t",
      "definition_spelling": "1:23-1:30",
      "definition_extent": "1:1-1:30",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:23-1:30|-1|1|4", "2:12-2:15|-1|1|4"]
    }, {
      "id": 1,
      "usr": 16985894625255407295,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 5,
      "definition_spelling": "2:12-2:15",
      "definition_extent": "2:1-5:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": []
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 439339022761937396,
      "detailed_name": "Foo::A",
      "short_name": "A",
      "hover": "Foo::A = 0",
      "declarations": [],
      "definition_spelling": "3:3-3:4",
      "definition_extent": "3:3-3:4",
      "variable_type": 1,
      "uses": [],
      "parent_id": 1,
      "parent_kind": 2,
      "kind": 15,
      "storage": 0
    }, {
      "id": 1,
      "usr": 15962370213938840720,
      "detailed_name": "Foo::B",
      "short_name": "B",
      "hover": "Foo::B = 20",
      "declarations": [],
      "definition_spelling": "4:3-4:4",
      "definition_extent": "4:3-4:9",
      "variable_type": 1,
      "uses": [],
      "parent_id": 1,
      "parent_kind": 2,
      "kind": 15,
      "storage": 0
    }]
}
*/
