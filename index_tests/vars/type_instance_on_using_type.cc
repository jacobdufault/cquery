struct S {};
using F = S;
void Foo() {
  F a;
}

// TODO: Should we also add a usage to |S|?

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 4750332761459066907,
      "detailed_name": "S",
      "short_name": "S",
      "kind": 6,
      "definition_spelling": "1:8-1:9",
      "definition_extent": "1:1-1:12",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:11-2:12|-1|1|4"]
    }, {
      "id": 1,
      "usr": 7434820806199665424,
      "detailed_name": "F",
      "short_name": "F",
      "kind": 11,
      "hover": "using F = S",
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:1-2:12",
      "alias_of": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["2:7-2:8|-1|1|4", "4:3-4:4|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 4654328188330986029,
      "detailed_name": "void Foo()",
      "short_name": "Foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:9",
      "definition_extent": "3:1-5:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 7730100248624586522,
      "detailed_name": "F a",
      "short_name": "a",
      "declarations": [],
      "definition_spelling": "4:5-4:6",
      "definition_extent": "4:3-4:6",
      "variable_type": 1,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
