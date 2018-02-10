struct Type {};
static Type t;
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 13487927231218873822,
      "detailed_name": "Type",
      "short_name": "Type",
      "kind": 6,
      "definition_spelling": "1:8-1:12",
      "definition_extent": "1:1-1:15",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["2:8-2:12|-1|1|4"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 6601831367240627080,
      "detailed_name": "Type t",
      "short_name": "t",
      "declarations": [],
      "definition_spelling": "2:13-2:14",
      "definition_extent": "2:1-2:14",
      "variable_type": 0,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 3
    }]
}
*/
