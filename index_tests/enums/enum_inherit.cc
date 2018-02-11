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
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 5,
      "spell": "1:6-1:9|-1|1|2",
      "extent": "1:1-4:2|-1|1|0",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [],
      "uses": []
    }, {
      "id": 1,
      "usr": 14939241684006947339,
      "detailed_name": "int32_t",
      "short_name": "int32_t",
      "kind": 11,
      "hover": "typedef int int32_t",
      "spell": "6:13-6:20|-1|1|2",
      "extent": "6:1-6:20|-1|1|0",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["6:13-6:20|-1|1|4", "8:12-8:13|-1|1|4"]
    }, {
      "id": 2,
      "usr": 2986879766914123941,
      "detailed_name": "E",
      "short_name": "E",
      "kind": 5,
      "spell": "8:12-8:13|-1|1|2",
      "extent": "8:1-11:2|-1|1|0",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [2, 3],
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
      "spell": "2:3-2:4|0|2|2",
      "extent": "2:3-2:4|0|2|2",
      "type": 0,
      "uses": [],
      "kind": 15,
      "storage": 0
    }, {
      "id": 1,
      "usr": 15962370213938840720,
      "detailed_name": "Foo::B",
      "short_name": "B",
      "hover": "Foo::B = 20",
      "declarations": [],
      "spell": "3:3-3:4|0|2|2",
      "extent": "3:3-3:9|0|2|2",
      "type": 0,
      "uses": [],
      "kind": 15,
      "storage": 0
    }, {
      "id": 2,
      "usr": 16614320383091394267,
      "detailed_name": "E::E0",
      "short_name": "E0",
      "hover": "E::E0 = 0",
      "declarations": [],
      "spell": "9:3-9:5|2|2|2",
      "extent": "9:3-9:5|2|2|2",
      "type": 2,
      "uses": [],
      "kind": 15,
      "storage": 0
    }, {
      "id": 3,
      "usr": 16847439761518576294,
      "detailed_name": "E::E20",
      "short_name": "E20",
      "hover": "E::E20 = 20",
      "declarations": [],
      "spell": "10:3-10:6|2|2|2",
      "extent": "10:3-10:11|2|2|2",
      "type": 2,
      "uses": [],
      "kind": 15,
      "storage": 0
    }]
}
*/
