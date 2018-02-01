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
      "detailed_name": "vector3",
      "short_name": "vector3",
      "kind": 10,
      "definition_spelling": "1:7-1:14",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [3],
      "instances": [],
      "uses": []
    }, {
      "id": 1,
      "usr": 1428566502523368801,
      "detailed_name": "vector3::(anon)",
      "short_name": "(anon)",
      "kind": 6,
      "definition_spelling": "2:3-2:9",
      "definition_extent": "2:3-2:28",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1, 2],
      "instances": [],
      "uses": []
    }, {
      "id": 2,
      "usr": 21,
      "detailed_name": "",
      "short_name": "",
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
      "detailed_name": "float vector3::(anon struct)::x",
      "short_name": "x",
      "declarations": [],
      "definition_spelling": "2:18-2:19",
      "definition_extent": "2:12-2:19",
      "variable_type": 2,
      "uses": [],
      "parent_id": 1,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 1,
      "usr": 4821094820988543895,
      "detailed_name": "float vector3::(anon struct)::y",
      "short_name": "y",
      "declarations": [],
      "definition_spelling": "2:21-2:22",
      "definition_extent": "2:12-2:22",
      "variable_type": 2,
      "uses": [],
      "parent_id": 1,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 2,
      "usr": 15292551660437765731,
      "detailed_name": "float vector3::(anon struct)::z",
      "short_name": "z",
      "declarations": [],
      "definition_spelling": "2:24-2:25",
      "definition_extent": "2:12-2:25",
      "variable_type": 2,
      "uses": [],
      "parent_id": 1,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 3,
      "usr": 1963212417280098348,
      "detailed_name": "float [3] vector3::v",
      "short_name": "v",
      "declarations": [],
      "definition_spelling": "3:9-3:10",
      "definition_extent": "3:3-3:13",
      "variable_type": 2,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }]
}
*/
