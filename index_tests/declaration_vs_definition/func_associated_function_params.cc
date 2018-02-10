int foo(int, int);
int foo(int aa,
        int bb);
int foo(int aaa, int bbb);
int foo(int a, int b) { return 0; }

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
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
      "usr": 2747674671862363334,
      "detailed_name": "int foo(int a, int b)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "1:5-1:8",
          "extent": "1:1-1:18",
          "content": "int foo(int, int)",
          "param_spellings": ["1:12-1:12", "1:17-1:17"]
        }, {
          "spelling": "2:5-2:8",
          "extent": "2:1-3:16",
          "content": "int foo(int aa,\n        int bb)",
          "param_spellings": ["2:13-2:15", "3:13-3:15"]
        }, {
          "spelling": "4:5-4:8",
          "extent": "4:1-4:26",
          "content": "int foo(int aaa, int bbb)",
          "param_spellings": ["4:13-4:16", "4:22-4:25"]
        }],
      "definition_spelling": "5:5-5:8",
      "definition_extent": "5:1-5:36",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 10480417713467708012,
      "detailed_name": "int a",
      "short_name": "a",
      "declarations": [],
      "definition_spelling": "5:13-5:14",
      "definition_extent": "5:9-5:14",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }, {
      "id": 1,
      "usr": 18099600680625658464,
      "detailed_name": "int b",
      "short_name": "b",
      "declarations": [],
      "definition_spelling": "5:20-5:21",
      "definition_extent": "5:16-5:21",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }]
}
*/
