struct Foo {
  static int x;
};

void accept(int);

void foo() {
  accept(Foo::x);
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
      "definition_spelling": "1:8-1:11",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:11", "8:10-8:13"]
    }, {
      "id": 1,
      "usr": 17,
      "short_name": "",
      "detailed_name": "",
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
      "usr": 17175780305784503374,
      "short_name": "accept",
      "detailed_name": "void accept(int)",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "5:6-5:12",
          "extent": "5:1-5:17",
          "content": "void accept(int)",
          "param_spellings": ["5:16-5:16"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@8:3-8:9"],
      "callees": []
    }, {
      "id": 1,
      "usr": 4259594751088586730,
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "7:6-7:9",
      "definition_extent": "7:1-9:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@8:3-8:9"]
    }],
  "vars": [{
      "id": 0,
      "usr": 8599782646965457351,
      "short_name": "x",
      "detailed_name": "int Foo::x",
      "kind": 21,
      "storage": 3,
      "declaration": "2:14-2:15",
      "variable_type": 1,
      "uses": ["2:14-2:15", "8:15-8:16"]
    }]
}
*/
