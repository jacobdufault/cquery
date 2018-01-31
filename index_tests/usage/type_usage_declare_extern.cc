struct T {};

extern T t;
/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 5673439900521455039,
      "detailed_name": "T",
      "short_name_offset": 0,
      "short_name_size": 1,
      "kind": 6,
      "definition_spelling": "1:8-1:9",
      "definition_extent": "1:1-1:12",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["3:8-3:9"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 1346710425945444872,
      "detailed_name": "T t",
      "short_name_offset": 2,
      "short_name_size": 1,
      "declarations": ["3:10-3:11"],
      "variable_type": 0,
      "uses": [],
      "parent_id": 18446744073709551615,
      "parent_kind": 0,
      "kind": 13,
      "storage": 2
    }]
}
*/
