struct Foo {
  void foo();
};

void usage() {
  Foo* f = nullptr;
  f->foo();
}
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
      "kind": 6,
      "definition_spelling": "1:8-1:11",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [0],
      "uses": ["6:3-6:6|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 17922201480358737771,
      "detailed_name": "void Foo::foo()",
      "short_name": "foo",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "2:8-2:11",
          "extent": "2:3-2:13",
          "content": "void foo()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["7:6-7:9|1|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 6767773193109753523,
      "detailed_name": "void usage()",
      "short_name": "usage",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:6-5:11",
      "definition_extent": "5:1-8:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["7:6-7:9|0|3|64"]
    }],
  "vars": [{
      "id": 0,
      "usr": 12410753116854389823,
      "detailed_name": "Foo *f",
      "short_name": "f",
      "hover": "Foo *f = nullptr",
      "declarations": [],
      "definition_spelling": "6:8-6:9",
      "definition_extent": "6:3-6:19",
      "variable_type": 0,
      "uses": ["7:3-7:4|1|3|4"],
      "parent_id": 1,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
