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
      "short_name": "Type",
      "detailed_name": "Type",
      "kind": 6,
      "definition_spelling": "1:8-1:12",
      "definition_extent": "1:1-1:15",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1, 2, 3, 4, 5],
      "uses": ["1:8-1:12", "3:10-3:14", "3:26-3:30", "4:3-4:7", "5:3-5:7", "6:9-6:13", "7:9-7:13"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 16858540520096802573,
      "short_name": "foo",
      "detailed_name": "void foo(Type &a0, const Type &a1)",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:9",
      "definition_extent": "3:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 16414210592877294238,
      "short_name": "a0",
      "detailed_name": "Type &a0",
      "kind": 25,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:16-3:18",
      "definition_extent": "3:10-3:18",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 1,
      "usr": 11558141642862804306,
      "short_name": "a1",
      "detailed_name": "const Type &a1",
      "kind": 25,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:32-3:34",
      "definition_extent": "3:20-3:34",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 2,
      "usr": 1536316608590232194,
      "short_name": "a2",
      "detailed_name": "Type a2",
      "kind": 13,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "4:8-4:10",
      "definition_extent": "4:3-4:10",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 3,
      "usr": 316760354845869406,
      "short_name": "a3",
      "detailed_name": "Type *a3",
      "kind": 13,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:9-5:11",
      "definition_extent": "5:3-5:11",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 4,
      "usr": 12321730890779907974,
      "short_name": "a4",
      "detailed_name": "const Type *a4",
      "kind": 13,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "6:15-6:17",
      "definition_extent": "6:3-6:17",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 5,
      "usr": 4771437488905761633,
      "short_name": "a5",
      "detailed_name": "const Type *const a5",
      "kind": 13,
      "storage": 1,
      "hover": "const Type *const a5 = nullptr",
      "declarations": [],
      "definition_spelling": "7:21-7:23",
      "definition_extent": "7:3-7:33",
      "variable_type": 0,
      "uses": []
    }]
}
*/
