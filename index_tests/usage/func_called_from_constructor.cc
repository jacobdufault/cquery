void called() {}

struct Foo {
  Foo();
};

Foo::Foo() {
  called();
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 6,
      "definition_spelling": "3:8-3:11",
      "definition_extent": "3:1-5:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [1],
      "vars": [],
      "instances": [],
      "uses": ["3:8-3:11", "4:3-4:6", "7:6-7:9", "7:1-7:4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 468307235068920063,
      "short_name": "called",
      "detailed_name": "void called()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "1:6-1:12",
      "definition_extent": "1:1-1:17",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@8:3-8:9"],
      "callees": []
    }, {
      "id": 1,
      "usr": 3385168158331140247,
      "short_name": "Foo",
      "detailed_name": "void Foo::Foo()",
      "kind": 22,
      "storage": 1,
      "declarations": [{
          "spelling": "4:3-4:6",
          "extent": "4:3-4:8",
          "content": "Foo()",
          "param_spellings": []
        }],
      "definition_spelling": "7:6-7:9",
      "definition_extent": "7:1-9:2",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@8:3-8:9"]
    }],
  "vars": []
}
*/
