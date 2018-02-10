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
      "usr": 3223559731210506952,
      "detailed_name": "void consume(void (*)())",
      "short_name": "consume",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:13",
      "definition_extent": "1:1-1:28",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["7:3-7:10|2|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 5264867802674151787,
      "detailed_name": "void used()",
      "short_name": "used",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:10",
      "definition_extent": "3:1-3:15",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["6:18-6:22|2|3|64", "7:12-7:16|2|3|64"],
      "callees": []
    }, {
      "id": 2,
      "usr": 9376923949268137283,
      "detailed_name": "void user()",
      "short_name": "user",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:6-5:10",
      "definition_extent": "5:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["6:18-6:22|1|3|64", "6:18-6:22|1|3|64", "7:3-7:10|0|3|64", "7:12-7:16|1|3|64"]
    }],
  "vars": [{
      "id": 0,
      "usr": 13681544683892648258,
      "detailed_name": "void (*)() x",
      "short_name": "x",
      "declarations": [],
      "definition_spelling": "6:10-6:11",
      "definition_extent": "6:3-6:22",
      "uses": [],
      "parent_id": 2,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
