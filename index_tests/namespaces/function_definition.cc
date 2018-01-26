namespace hello {
void foo() {}
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 2029211996748007610,
      "short_name": "hello",
      "detailed_name": "hello",
      "kind": 2,
      "definition_spelling": "1:11-1:16",
      "definition_extent": "1:1-3:2",
      "parents": [1],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["1:11-1:16"]
    }, {
      "id": 1,
      "usr": 13838176792705659279,
      "short_name": "<fundamental>",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [0],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 243328841292951622,
      "short_name": "foo",
      "detailed_name": "void hello::foo()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "2:6-2:9",
      "definition_extent": "2:1-2:14",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
*/
