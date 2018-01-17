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
      "is_operator": false,
      "usr": 4259594751088586730,
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
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
      "is_operator": false,
      "usr": 1328781044864682611,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
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
      "short_name": "x",
      "detailed_name": "int x",
      "definition_spelling": "2:7-2:8",
      "definition_extent": "2:3-2:8",
      "kind": 13,
      "uses": ["2:7-2:8", "5:7-5:8", "4:24-4:25"]
    }, {
      "id": 1,
      "usr": 16806544259835773270,
      "short_name": "dosomething",
      "detailed_name": "lambda dosomething",
      "hover": "lambda dosomething = [&x](int y) {\r\n    ++x;\r\n    ++y;\r\n  }",
      "definition_spelling": "4:8-4:19",
      "definition_extent": "4:3-7:4",
      "variable_type": 0,
      "kind": 13,
      "uses": ["4:8-4:19", "9:3-9:14", "10:3-10:14", "11:3-11:14"]
    }, {
      "id": 2,
      "usr": 2034725908368218782,
      "short_name": "y",
      "detailed_name": "int y",
      "definition_spelling": "4:31-4:32",
      "definition_extent": "4:27-4:32",
      "kind": 25,
      "uses": ["4:31-4:32", "6:7-6:8"]
    }]
}
*/
