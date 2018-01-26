union vector3 {
  struct { float x, y, z; };
  float v[3];
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17937907487590875128,
      "short_name": "vector3",
      "detailed_name": "vector3",
      "kind": 10,
      "definition_spelling": "1:7-1:14",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [3],
      "instances": [],
      "uses": ["1:7-1:14"]
    }, {
      "id": 1,
      "usr": 1428566502523368801,
      "short_name": "(anon)",
      "detailed_name": "vector3::(anon)",
      "kind": 6,
      "definition_spelling": "2:3-2:9",
      "definition_extent": "2:3-2:28",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1, 2],
      "instances": [],
      "uses": ["2:3-2:9"]
    }, {
      "id": 2,
      "usr": 21,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1, 2, 3],
      "uses": []
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 3348817847649945564,
      "short_name": "x",
      "detailed_name": "float vector3::(anon struct)::x",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "2:18-2:19",
      "definition_extent": "2:12-2:19",
      "variable_type": 2,
      "declaring_type": 1,
      "uses": ["2:18-2:19"]
    }, {
      "id": 1,
      "usr": 4821094820988543895,
      "short_name": "y",
      "detailed_name": "float vector3::(anon struct)::y",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "2:21-2:22",
      "definition_extent": "2:12-2:22",
      "variable_type": 2,
      "declaring_type": 1,
      "uses": ["2:21-2:22"]
    }, {
      "id": 2,
      "usr": 15292551660437765731,
      "short_name": "z",
      "detailed_name": "float vector3::(anon struct)::z",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "2:24-2:25",
      "definition_extent": "2:12-2:25",
      "variable_type": 2,
      "declaring_type": 1,
      "uses": ["2:24-2:25"]
    }, {
      "id": 3,
      "usr": 1963212417280098348,
      "short_name": "v",
      "detailed_name": "float [3] vector3::v",
      "kind": 14,
      "storage": 0,
      "definition_spelling": "3:9-3:10",
      "definition_extent": "3:3-3:13",
      "variable_type": 2,
      "declaring_type": 0,
      "uses": ["3:9-3:10"]
    }]
}
*/
