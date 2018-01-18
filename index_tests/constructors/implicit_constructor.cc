struct Type {
  Type() {}
};

void Make() {
  Type foo0;
  auto foo1 = Type();
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
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [0, 1],
      "uses": ["1:8-1:12", "2:3-2:7", "6:3-6:7", "7:15-7:19"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 10530961286677896857,
      "short_name": "Type",
      "detailed_name": "void Type::Type()",
      "kind": 22,
      "declarations": [],
      "definition_spelling": "2:3-2:7",
      "definition_extent": "2:3-2:12",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["~1@6:8-6:12", "1@7:15-7:19"],
      "callees": []
    }, {
      "id": 1,
      "is_operator": false,
      "usr": 3957104924306079513,
      "short_name": "Make",
      "detailed_name": "void Make()",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "5:6-5:10",
      "definition_extent": "5:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["~0@6:8-6:12", "0@7:15-7:19"]
    }],
  "vars": [{
      "id": 0,
      "usr": 449111627548814328,
      "short_name": "foo0",
      "detailed_name": "Type foo0",
      "definition_spelling": "6:8-6:12",
      "definition_extent": "6:3-6:12",
      "variable_type": 0,
      "kind": 13,
      "uses": ["6:8-6:12"]
    }, {
      "id": 1,
      "usr": 17097499197730163115,
      "short_name": "foo1",
      "detailed_name": "Type foo1",
      "definition_spelling": "7:8-7:12",
      "definition_extent": "7:3-7:21",
      "variable_type": 0,
      "kind": 13,
      "uses": ["7:8-7:12"]
    }]
}
*/
