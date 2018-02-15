class Foo {
  static int foo;
};

int Foo::foo;

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
      "kind": 7,
      "spell": "1:7-1:10|-1|1|2",
      "extent": "1:1-3:2|-1|1|0",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0],
      "instances": [],
      "uses": ["5:5-5:8|-1|1|4"]
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
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 8942920329766232482,
      "detailed_name": "int Foo::foo",
      "short_name": "foo",
      "declarations": ["2:14-2:17|0|2|2"],
      "spell": "5:10-5:13|0|2|2",
      "extent": "5:1-5:13|-1|1|0",
      "type": 1,
      "uses": [],
      "kind": 21,
      "storage": 1
    }]
}
*/
