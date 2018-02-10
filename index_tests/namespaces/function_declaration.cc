namespace hello {
void foo(int a, int b);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 2029211996748007610,
      "detailed_name": "hello",
      "short_name": "hello",
      "kind": 2,
      "definition_spelling": "1:11-1:16",
      "definition_extent": "1:1-3:2",
      "parents": [1],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["1:11-1:16|-1|1|4"]
    }, {
      "id": 1,
      "usr": 13838176792705659279,
      "detailed_name": "<fundamental>",
      "short_name": "",
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
      "usr": 18343102288837190527,
      "detailed_name": "void hello::foo(int a, int b)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "2:6-2:9",
          "extent": "2:1-2:23",
          "content": "void foo(int a, int b)",
          "param_spellings": ["2:14-2:15", "2:21-2:22"]
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
