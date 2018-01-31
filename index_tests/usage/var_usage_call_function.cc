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
      "detailed_name": "void called()",
      "short_name_offset": 5,
      "short_name_size": 6,
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
      "detailed_name": "void caller()",
      "short_name_offset": 5,
      "short_name_size": 6,
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
      "detailed_name": "void (*)() x",
      "short_name_offset": 11,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "4:8-4:9",
      "definition_extent": "4:3-4:19",
      "uses": ["5:3-5:4"],
      "parent_id": 1,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
