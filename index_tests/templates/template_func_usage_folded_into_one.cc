template<typename T>
static int foo() {
  return 3;
}

int a = foo<int>();
int b = foo<bool>();

// TODO: put template foo inside a namespace
// TODO: put template foo inside a template class inside a namespace

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 326583651986177228,
      "short_name": "foo",
      "detailed_name": "int foo()",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "2:12-2:15",
      "definition_extent": "2:1-4:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["-1@6:9-6:12", "-1@7:9-7:12"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 16721564935990383768,
      "short_name": "a",
      "detailed_name": "int a",
      "hover": "int a = foo<int>()",
      "definition_spelling": "6:5-6:6",
      "definition_extent": "6:1-6:19",
      "variable_type": 0,
      "kind": 13,
      "uses": ["6:5-6:6"]
    }, {
      "id": 1,
      "usr": 12028309045033782423,
      "short_name": "b",
      "detailed_name": "int b",
      "hover": "int b = foo<bool>()",
      "definition_spelling": "7:5-7:6",
      "definition_extent": "7:1-7:20",
      "variable_type": 0,
      "kind": 13,
      "uses": ["7:5-7:6"]
    }]
}
*/
