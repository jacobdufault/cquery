struct Foo;

void foo(Foo* f, Foo*);
void foo(Foo* f, Foo*) {}

/*
// TODO: No interesting usage on prototype. But maybe that's ok!
// TODO: We should have the same variable declared for both prototype and
//       declaration. So it should have a usage marker on both. Then we could
//       rename parameters!

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
      "instances": [0],
      "uses": ["1:8-1:11|-1|1|4", "3:10-3:13|-1|1|4", "3:18-3:21|-1|1|4", "4:10-4:13|-1|1|4", "4:18-4:21|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 8908726657907936744,
      "detailed_name": "void foo(Foo *f, Foo *)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "3:6-3:9",
          "extent": "3:1-3:23",
          "content": "void foo(Foo* f, Foo*)",
          "param_spellings": ["3:15-3:16", "3:22-3:22"]
        }],
      "definition_spelling": "4:6-4:9",
      "definition_extent": "4:1-4:26",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 2161866804398917919,
      "detailed_name": "Foo *f",
      "short_name": "f",
      "declarations": [],
      "definition_spelling": "4:15-4:16",
      "definition_extent": "4:10-4:16",
      "variable_type": 0,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }]
}
*/
