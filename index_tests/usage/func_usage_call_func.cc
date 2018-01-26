void called() {}
void caller() {
  called();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 468307235068920063,
      "short_name": "called",
      "detailed_name": "void called()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:12",
      "definition_extent": "1:1-1:17",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@3:3-3:9"],
      "callees": []
    }, {
      "id": 1,
      "is_operator": false,
      "usr": 11404881820527069090,
      "short_name": "caller",
      "detailed_name": "void caller()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "2:6-2:12",
      "definition_extent": "2:1-4:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@3:3-3:9"]
    }],
  "vars": []
}
*/
