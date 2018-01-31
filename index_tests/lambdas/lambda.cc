void foo() {
  int x;

  auto dosomething = [&x](int y) {
    ++x;
    ++y;
  };

  dosomething(1);
  dosomething(1);
  dosomething(1);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 2],
      "uses": []
    }, {
      "id": 1,
      "usr": 1287417953265234030,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [1],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name_offset": 5,
      "short_name_size": 3,
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:9",
      "definition_extent": "1:1-12:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["1@9:14-9:15", "1@10:14-10:15", "1@11:14-11:15"]
    }, {
      "id": 1,
      "usr": 1328781044864682611,
      "detailed_name": "",
      "short_name_offset": 8,
      "short_name_size": 0,
      "kind": 0,
      "storage": 0,
      "declarations": [],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["0@9:14-9:15", "0@10:14-10:15", "0@11:14-11:15"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 17270098654620601683,
      "detailed_name": "int x",
      "short_name_offset": 4,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "variable_type": 0,
      "uses": ["5:7-5:8", "4:24-4:25"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 16806544259835773270,
      "detailed_name": "lambda dosomething",
      "short_name_offset": 7,
      "short_name_size": 11,
      "declarations": [],
      "definition_spelling": "4:8-4:19",
      "definition_extent": "4:3-7:4",
      "variable_type": 1,
      "uses": ["9:3-9:14", "10:3-10:14", "11:3-11:14"],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 2,
      "usr": 2034725908368218782,
      "detailed_name": "int y",
      "short_name_offset": 4,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "4:31-4:32",
      "definition_extent": "4:27-4:32",
      "variable_type": 0,
      "uses": ["6:7-6:8"],
      "parent_id": 18446744073709551615,
      "parent_kind": 0,
      "kind": 25,
      "storage": 1
    }]
}
*/
