template<typename T>
void accept(T);

void foo() {
  accept(1);
  accept(true);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 13420564603121289209,
      "detailed_name": "T",
      "short_name": "T",
      "kind": 25,
      "definition_spelling": "1:19-1:20",
      "definition_extent": "1:10-1:20",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:13-2:14|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 10585861037135727329,
      "detailed_name": "void accept(T)",
      "short_name": "accept",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "2:6-2:12",
          "extent": "2:1-2:15",
          "content": "void accept(T)",
          "param_spellings": ["2:14-2:14"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["5:3-5:9|1|3|64", "6:3-6:9|1|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "4:6-4:9",
      "definition_extent": "4:1-7:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["5:3-5:9|0|3|64", "6:3-6:9|0|3|64"]
    }],
  "vars": []
}
*/
