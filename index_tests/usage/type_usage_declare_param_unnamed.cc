struct ForwardType;
void foo(ForwardType*) {}
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 13749354388332789217,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:19", "2:10-2:21"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 15327735280790448926,
      "short_name": "foo",
      "detailed_name": "void foo(ForwardType *)",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "2:6-2:9",
      "definition_extent": "2:1-2:26",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
*/
