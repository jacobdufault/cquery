class Foo {
  int foo;
};

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
      "kind": 7,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0],
      "instances": [],
      "uses": ["1:7-1:10"]
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
      "instances": [0],
      "uses": []
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 9736582033442720743,
      "short_name": "foo",
      "detailed_name": "int Foo::foo",
      "kind": 14,
      "storage": 0,
      "declarations": [],
      "definition_spelling": "2:7-2:10",
      "definition_extent": "2:3-2:10",
      "variable_type": 1,
      "declaring_type": 0,
      "uses": []
    }]
}
*/
