template<typename T>
class unique_ptr {};

struct S {};

static unique_ptr<bool> f0;
static unique_ptr<S> f1;

unique_ptr<S>* return_type() {
  unique_ptr<S>* local;
  return nullptr;
}
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
      "instances": [0, 1, 2],
      "uses": ["2:7-2:17", "6:8-6:18", "7:8-7:18", "9:1-9:11", "10:3-10:13"]
    }, {
      "id": 1,
      "usr": 4750332761459066907,
      "short_name": "S",
      "detailed_name": "S",
      "kind": 6,
      "definition_spelling": "4:8-4:9",
      "definition_extent": "4:1-4:12",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["4:8-4:9", "7:19-7:20", "9:12-9:13", "10:14-10:15"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 16359708726068806331,
      "short_name": "return_type",
      "detailed_name": "unique_ptr<S> *return_type()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "9:16-9:27",
      "definition_extent": "9:1-12:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 12857919739649552168,
      "short_name": "f0",
      "detailed_name": "unique_ptr<bool> f0",
      "kind": 13,
      "storage": 3,
      "declarations": [],
      "definition_spelling": "6:25-6:27",
      "definition_extent": "6:1-6:27",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 1,
      "usr": 18075066956054788088,
      "short_name": "f1",
      "detailed_name": "unique_ptr<S> f1",
      "kind": 13,
      "storage": 3,
      "declarations": [],
      "definition_spelling": "7:22-7:24",
      "definition_extent": "7:1-7:24",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 2,
      "usr": 2462000803278878465,
      "short_name": "local",
      "detailed_name": "unique_ptr<S> *local",
      "kind": 13,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "10:18-10:23",
      "definition_extent": "10:3-10:23",
      "variable_type": 0,
      "uses": []
    }]
}
*/
