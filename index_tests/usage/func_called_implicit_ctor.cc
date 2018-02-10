struct Wrapper {
  Wrapper(int i);
};

int called() { return 1; }

Wrapper caller() {
  return called();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 13611487872560323389,
      "detailed_name": "Wrapper",
      "short_name": "Wrapper",
      "kind": 6,
      "definition_spelling": "1:8-1:15",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["2:3-2:10|0|3|4", "7:1-7:8|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 10544127002917214589,
      "detailed_name": "void Wrapper::Wrapper(int i)",
      "short_name": "Wrapper",
      "kind": 22,
      "storage": 1,
      "declarations": [{
          "spelling": "2:3-2:10",
          "extent": "2:3-2:17",
          "content": "Wrapper(int i)",
          "param_spellings": ["2:15-2:16"]
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["8:10-8:18|2|3|72"],
      "callees": []
    }, {
      "id": 1,
      "usr": 468307235068920063,
      "detailed_name": "int called()",
      "short_name": "called",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:5-5:11",
      "definition_extent": "5:1-5:27",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["8:10-8:16|2|3|64"],
      "callees": []
    }, {
      "id": 2,
      "usr": 11404881820527069090,
      "detailed_name": "Wrapper caller()",
      "short_name": "caller",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "7:9-7:15",
      "definition_extent": "7:1-9:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["8:10-8:18|0|3|72", "8:10-8:16|1|3|64"]
    }],
  "vars": []
}
*/
