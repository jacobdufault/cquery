#define FOO(aaa, bbb) \
  int a();\
  int a() { return aaa + bbb; }


int make1() {
  return 3;
}
const int make2 = 5;


FOO(make1(), make2);

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 14400399977994209582,
      "detailed_name": "int make1()",
      "short_name": "make1",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "6:5-6:10",
      "definition_extent": "6:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["12:5-12:10|1|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 9720930732776154610,
      "detailed_name": "int a()",
      "short_name": "a",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "12:1-12:20",
          "extent": "12:1-12:20",
          "content": "int a();\n  int a() { return aaa + bbb; }\n\n\nint make1() {\n  return 3;\n}\nconst int make2 = 5;\n\n\nFOO(make1(), make2)",
          "param_spellings": []
        }],
      "definition_spelling": "12:1-12:20",
      "definition_extent": "12:1-12:20",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["12:5-12:10|0|3|64"]
    }],
  "vars": [{
      "id": 0,
      "usr": 2878407290385495202,
      "detailed_name": "const int make2",
      "short_name": "make2",
      "hover": "const int make2 = 5",
      "declarations": [],
      "definition_spelling": "9:11-9:16",
      "definition_extent": "9:1-9:20",
      "variable_type": 0,
      "uses": ["12:14-12:19|1|3|4"],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 4261071340275951718,
      "detailed_name": "FOO",
      "short_name": "FOO",
      "hover": "#define FOO(aaa, bbb)\n  int a();\n  int a() { return aaa + bbb; }",
      "declarations": [],
      "definition_spelling": "1:9-1:12",
      "definition_extent": "1:9-3:32",
      "uses": ["12:1-12:4|-1|1|4"],
      "parent_kind": 0,
      "kind": 4,
      "storage": 0
    }]
}
*/