class Foo {
  void foo();
};

/*
// NOTE: Lack of declaring_type in functions and funcs in Foo is okay, because
//       those are processed when we find the definition for Foo::foo. Pure
//       virtuals are treated specially and get added to the type immediately.

OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": []
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
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
