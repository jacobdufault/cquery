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
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:19|-1|1|4", "2:10-2:21|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 15327735280790448926,
      "detailed_name": "void foo(ForwardType *)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "2:6-2:9",
      "definition_extent": "2:1-2:26",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
