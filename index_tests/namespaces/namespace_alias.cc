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
  "types": [],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 10818727483146447186,
      "short_name": "func",
      "detailed_name": "void func()",
      "kind": 12,
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
      "definition_spelling": "4:18-4:21",
      "definition_extent": "4:14-4:26",
      "kind": 13,
      "uses": ["4:18-4:21", "12:26-12:29", "13:16-13:19"]
    }, {
      "id": 1,
      "usr": 107714981785063096,
      "short_name": "a",
      "detailed_name": "int a",
      "hover": "int a = foo::bar::baz::qux",
      "definition_spelling": "12:7-12:8",
      "definition_extent": "12:3-12:29",
      "kind": 13,
      "uses": ["12:7-12:8"]
    }, {
      "id": 2,
      "usr": 1200087780658383286,
      "short_name": "b",
      "detailed_name": "int b",
      "hover": "int b = fbz::qux",
      "definition_spelling": "13:7-13:8",
      "definition_extent": "13:3-13:19",
      "kind": 13,
      "uses": ["13:7-13:8"]
    }]
}
*/
