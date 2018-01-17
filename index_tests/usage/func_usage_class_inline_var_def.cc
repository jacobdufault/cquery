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
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 7,
      "definition_spelling": "5:7-5:10",
      "definition_extent": "5:1-7:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0],
      "instances": [],
      "uses": ["5:7-5:10"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 9630503130605430498,
      "short_name": "helper",
      "detailed_name": "int helper()",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "1:12-1:18",
      "definition_extent": "1:1-3:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["-1@6:11-6:17"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 4220150017963593039,
      "short_name": "x",
      "detailed_name": "int Foo::x",
      "hover": "int Foo::x = helper()",
      "definition_spelling": "6:7-6:8",
      "definition_extent": "6:3-6:19",
      "declaring_type": 0,
      "kind": 14,
      "uses": ["6:7-6:8"]
    }]
}
*/
