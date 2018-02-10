struct Type {};

void foo(Type& a0, const Type& a1) {
  Type a2;
  Type* a3;
  const Type* a4;
  const Type* const a5 = nullptr;
}
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
      "instances": [0, 1, 2, 3, 4, 5],
      "uses": ["3:10-3:14|-1|1|4", "3:26-3:30|-1|1|4", "4:3-4:7|-1|1|4", "5:3-5:7|-1|1|4", "6:9-6:13|-1|1|4", "7:9-7:13|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 16858540520096802573,
      "detailed_name": "void foo(Type &a0, const Type &a1)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:9",
      "definition_extent": "3:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 16414210592877294238,
      "detailed_name": "Type &a0",
      "short_name": "a0",
      "declarations": [],
      "definition_spelling": "3:16-3:18",
      "definition_extent": "3:10-3:18",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }, {
      "id": 1,
      "usr": 11558141642862804306,
      "detailed_name": "const Type &a1",
      "short_name": "a1",
      "declarations": [],
      "definition_spelling": "3:32-3:34",
      "definition_extent": "3:20-3:34",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }, {
      "id": 2,
      "usr": 1536316608590232194,
      "detailed_name": "Type a2",
      "short_name": "a2",
      "declarations": [],
      "definition_spelling": "4:8-4:10",
      "definition_extent": "4:3-4:10",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 3,
      "usr": 316760354845869406,
      "detailed_name": "Type *a3",
      "short_name": "a3",
      "declarations": [],
      "definition_spelling": "5:9-5:11",
      "definition_extent": "5:3-5:11",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 4,
      "usr": 12321730890779907974,
      "detailed_name": "const Type *a4",
      "short_name": "a4",
      "declarations": [],
      "definition_spelling": "6:15-6:17",
      "definition_extent": "6:3-6:17",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 5,
      "usr": 4771437488905761633,
      "detailed_name": "const Type *const a5",
      "short_name": "a5",
      "hover": "const Type *const a5 = nullptr",
      "declarations": [],
      "definition_spelling": "7:21-7:23",
      "definition_extent": "7:3-7:33",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
