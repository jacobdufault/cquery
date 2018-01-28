namespace foo {
    namespace bar {
         namespace baz {
             int qux = 42;
         }
    }
}

namespace fbz = foo::bar::baz;

void func() {
  int a = foo::bar::baz::qux;
  int b = fbz::qux;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 926793467007732869,
      "short_name": "foo",
      "detailed_name": "foo",
      "kind": 2,
      "definition_spelling": "1:11-1:14",
      "definition_extent": "1:1-7:2",
      "parents": [1],
      "derived": [2],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:11-1:14", "9:17-9:20", "12:11-12:14", "12:11-12:14"]
    }, {
      "id": 1,
      "usr": 13838176792705659279,
      "short_name": "<fundamental>",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [0],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": []
    }, {
      "id": 2,
      "usr": 17805385787823406700,
      "short_name": "bar",
      "detailed_name": "foo::bar",
      "kind": 2,
      "definition_spelling": "2:15-2:18",
      "definition_extent": "2:5-6:6",
      "parents": [0],
      "derived": [3],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:15-2:18", "9:22-9:25", "12:16-12:19", "12:16-12:19"]
    }, {
      "id": 3,
      "usr": 14450849931009540802,
      "short_name": "baz",
      "detailed_name": "foo::bar::baz",
      "kind": 2,
      "definition_spelling": "3:20-3:23",
      "definition_extent": "3:10-5:11",
      "parents": [2],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0],
      "instances": [],
      "uses": ["3:20-3:23", "9:27-9:30", "12:21-12:24", "12:21-12:24"]
    }, {
      "id": 4,
      "usr": 17,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1, 2],
      "uses": []
    }, {
      "id": 5,
      "usr": 11879713791858506216,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["13:11-13:14", "13:11-13:14"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 10818727483146447186,
      "short_name": "func",
      "detailed_name": "void func()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "11:6-11:10",
      "definition_extent": "11:1-14:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 15042442838933090518,
      "short_name": "qux",
      "detailed_name": "int foo::bar::baz::qux",
      "hover": "int foo::bar::baz::qux = 42",
      "declarations": [],
      "definition_spelling": "4:18-4:21",
      "definition_extent": "4:14-4:26",
      "variable_type": 4,
      "uses": ["12:26-12:29", "13:16-13:19"],
      "parent_id": 3,
      "parent_kind": 2,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 107714981785063096,
      "short_name": "a",
      "detailed_name": "int a",
      "hover": "int a = foo::bar::baz::qux",
      "declarations": [],
      "definition_spelling": "12:7-12:8",
      "definition_extent": "12:3-12:29",
      "variable_type": 4,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }, {
      "id": 2,
      "usr": 1200087780658383286,
      "short_name": "b",
      "detailed_name": "int b",
      "hover": "int b = fbz::qux",
      "declarations": [],
      "definition_spelling": "13:7-13:8",
      "definition_extent": "13:3-13:19",
      "variable_type": 4,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
