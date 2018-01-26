void consume(void (*)()) {}

void used() {}

void user() {
  void (*x)() = &used;
  consume(&used);
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
      "usr": 3223559731210506952,
      "short_name": "consume",
      "detailed_name": "void consume(void (*)())",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:13",
      "definition_extent": "1:1-1:28",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["2@7:3-7:10"],
      "callees": []
    }, {
      "id": 1,
      "is_operator": false,
      "usr": 5264867802674151787,
      "short_name": "used",
      "detailed_name": "void used()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:10",
      "definition_extent": "3:1-3:15",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["2@6:18-6:22", "2@7:12-7:16"],
      "callees": []
    }, {
      "id": 2,
      "is_operator": false,
      "usr": 9376923949268137283,
      "short_name": "user",
      "detailed_name": "void user()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:6-5:10",
      "definition_extent": "5:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["1@6:18-6:22", "0@7:3-7:10", "1@7:12-7:16"]
    }],
  "vars": [{
      "id": 0,
      "usr": 13681544683892648258,
      "short_name": "x",
      "detailed_name": "void (*)() x",
      "kind": 13,
      "storage": 1,
      "definition_spelling": "6:10-6:11",
      "definition_extent": "6:3-6:22",
      "uses": ["6:10-6:11"]
    }]
}
*/
