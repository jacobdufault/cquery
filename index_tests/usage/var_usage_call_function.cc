void called() {}

void caller() {
  auto x = &called;
  x();

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
      "callers": ["1@4:13-4:19", "1@7:3-7:9"],
      "callees": []
    }, {
      "id": 1,
      "usr": 11404881820527069090,
      "short_name": "caller",
      "detailed_name": "void caller()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:12",
      "definition_extent": "3:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@4:13-4:19", "0@7:3-7:9"]
    }],
  "vars": [{
      "id": 0,
      "usr": 3510529098767253033,
      "short_name": "x",
      "detailed_name": "void (*)() x",
      "kind": 13,
      "storage": 1,
      "definition_spelling": "4:8-4:9",
      "definition_extent": "4:3-4:19",
      "uses": ["4:8-4:9", "5:3-5:4"]
    }]
}
*/
