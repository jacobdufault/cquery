static int helper() {
  return 5;
}

class Foo {
  int x = helper();
};

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
      "definition_spelling": "5:7-5:10",
      "definition_extent": "5:1-7:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0],
      "instances": [],
      "uses": []
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
      "usr": 9630503130605430498,
      "detailed_name": "int helper()",
      "short_name": "helper",
      "kind": 12,
      "storage": 3,
      "declarations": [],
      "definition_spelling": "1:12-1:18",
      "definition_extent": "1:1-3:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["6:11-6:17|0|2|64"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 4220150017963593039,
      "detailed_name": "int Foo::x",
      "short_name": "x",
      "hover": "int Foo::x = helper()",
      "declarations": [],
      "definition_spelling": "6:7-6:8",
      "definition_extent": "6:3-6:19",
      "variable_type": 1,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }]
}
*/
