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
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": ["1:8-1:11|-1|1|4", "3:10-3:13|-1|1|4", "3:19-3:22|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 8908726657907936744,
      "detailed_name": "void foo(Foo *p0, Foo *p1)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:6-3:9",
      "definition_extent": "3:1-3:30",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 4580260577538694711,
      "detailed_name": "Foo *p0",
      "short_name": "p0",
      "declarations": [],
      "definition_spelling": "3:15-3:17",
      "definition_extent": "3:10-3:17",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }, {
      "id": 1,
      "usr": 12071725611268840435,
      "detailed_name": "Foo *p1",
      "short_name": "p1",
      "declarations": [],
      "definition_spelling": "3:24-3:26",
      "definition_extent": "3:19-3:26",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }]
}
*/
