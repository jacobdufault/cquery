struct Foo;

void foo(Foo* p0, Foo* p1) {}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": ["1:8-1:11", "3:10-3:13", "3:19-3:22"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 8908726657907936744,
      "short_name": "foo",
      "detailed_name": "void foo(Foo *p0, Foo *p1)",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "3:6-3:9",
      "definition_extent": "3:1-3:30",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 4580260577538694711,
      "short_name": "p0",
      "detailed_name": "Foo *p0",
      "definition_spelling": "3:15-3:17",
      "definition_extent": "3:10-3:17",
      "variable_type": 0,
      "kind": 25,
      "uses": ["3:15-3:17"]
    }, {
      "id": 1,
      "usr": 12071725611268840435,
      "short_name": "p1",
      "detailed_name": "Foo *p1",
      "definition_spelling": "3:24-3:26",
      "definition_extent": "3:19-3:26",
      "variable_type": 0,
      "kind": 25,
      "uses": ["3:24-3:26"]
    }]
}
*/
