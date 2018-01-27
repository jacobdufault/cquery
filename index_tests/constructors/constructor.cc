class Foo {
public:
  Foo() {}
};

void foo() {
  Foo f;
  Foo* f2 = new Foo();
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
      "kind": 7,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-4:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [0, 1],
      "uses": ["1:7-1:10", "3:3-3:6", "7:3-7:6", "8:3-8:6", "8:17-8:20"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 3385168158331140247,
      "short_name": "Foo",
      "detailed_name": "void Foo::Foo()",
      "kind": 22,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:3-3:6",
      "definition_extent": "3:3-3:11",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["~1@7:7-7:8", "1@8:17-8:20"],
      "callees": []
    }, {
      "id": 1,
      "usr": 4259594751088586730,
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "6:6-6:9",
      "definition_extent": "6:1-9:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["~0@7:7-7:8", "0@8:17-8:20"]
    }],
  "vars": [{
      "id": 0,
      "usr": 18410644574635149442,
      "short_name": "f",
      "detailed_name": "Foo f",
      "kind": 13,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "7:7-7:8",
      "definition_extent": "7:3-7:8",
      "variable_type": 0,
      "uses": []
    }, {
      "id": 1,
      "usr": 11468802633764653592,
      "short_name": "f2",
      "detailed_name": "Foo *f2",
      "kind": 13,
      "storage": 1,
      "hover": "Foo *f2 = new Foo()",
      "declarations": [],
      "definition_spelling": "8:8-8:10",
      "definition_extent": "8:3-8:22",
      "variable_type": 0,
      "uses": []
    }]
}
*/
