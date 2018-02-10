class Parent {};
class Derived : public Parent {};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 3866412049634585509,
      "detailed_name": "Parent",
      "short_name": "Parent",
      "kind": 7,
      "definition_spelling": "1:7-1:13",
      "definition_extent": "1:1-1:16",
      "parents": [],
      "derived": [1],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:24-2:30|-1|1|4"]
    }, {
      "id": 1,
      "usr": 10963370434658308541,
      "detailed_name": "Derived",
      "short_name": "Derived",
      "kind": 7,
      "definition_spelling": "2:7-2:14",
      "definition_extent": "2:1-2:33",
      "parents": [0],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": []
    }],
  "funcs": [],
  "vars": []
}
*/
