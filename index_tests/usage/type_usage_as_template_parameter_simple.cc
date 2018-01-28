template<typename T>
class unique_ptr {};

struct S;

static unique_ptr<S> foo;

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 3286534761799572592,
      "short_name": "unique_ptr",
      "detailed_name": "unique_ptr",
      "kind": 7,
      "definition_spelling": "2:7-2:17",
      "definition_extent": "2:1-2:20",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["2:7-2:17", "6:8-6:18"]
    }, {
      "id": 1,
      "usr": 4750332761459066907,
      "short_name": "S",
      "detailed_name": "S",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["4:8-4:9", "6:19-6:20"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 3398408600781120939,
      "short_name": "foo",
      "detailed_name": "unique_ptr<S> foo",
      "declarations": [],
      "definition_spelling": "6:22-6:25",
      "definition_extent": "6:1-6:25",
      "variable_type": 0,
      "uses": [],
      "parent_id": 18446744073709551615,
      "parent_kind": 0,
      "kind": 13,
      "storage": 3
    }]
}
*/
