class Root {};
class MiddleA : public Root {};
class MiddleB : public Root {};
class Derived : public MiddleA, public MiddleB {};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 3897841498936210886,
      "short_name": "Root",
      "detailed_name": "Root",
      "kind": 7,
      "definition_spelling": "1:7-1:11",
      "definition_extent": "1:1-1:14",
      "parents": [],
      "derived": [1, 2],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:7-1:11", "2:24-2:28", "3:24-3:28"]
    }, {
      "id": 1,
      "usr": 11863524815063131483,
      "short_name": "MiddleA",
      "detailed_name": "MiddleA",
      "kind": 7,
      "definition_spelling": "2:7-2:14",
      "definition_extent": "2:1-2:31",
      "parents": [0],
      "derived": [3],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:7-2:14", "4:24-4:31"]
    }, {
      "id": 2,
      "usr": 14022569716337624303,
      "short_name": "MiddleB",
      "detailed_name": "MiddleB",
      "kind": 7,
      "definition_spelling": "3:7-3:14",
      "definition_extent": "3:1-3:31",
      "parents": [0],
      "derived": [3],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["3:7-3:14", "4:40-4:47"]
    }, {
      "id": 3,
      "usr": 10963370434658308541,
      "short_name": "Derived",
      "detailed_name": "Derived",
      "kind": 7,
      "definition_spelling": "4:7-4:14",
      "definition_extent": "4:1-4:50",
      "parents": [1, 2],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["4:7-4:14"]
    }],
  "funcs": [],
  "vars": []
}
*/
