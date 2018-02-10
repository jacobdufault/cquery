class Root {
  virtual void foo();
};
class Derived : public Root {
  void foo() override {}
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 3897841498936210886,
      "detailed_name": "Root",
      "short_name": "Root",
      "kind": 7,
      "definition_spelling": "1:7-1:11",
      "definition_extent": "1:1-3:2",
      "parents": [],
      "derived": [1],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["4:24-4:28|-1|1|4"]
    }, {
      "id": 1,
      "usr": 10963370434658308541,
      "detailed_name": "Derived",
      "short_name": "Derived",
      "kind": 7,
      "definition_spelling": "4:7-4:14",
      "definition_extent": "4:1-6:2",
      "parents": [0],
      "derived": [],
      "types": [],
      "funcs": [1],
      "vars": [],
      "instances": [],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 9948027785633571339,
      "detailed_name": "void Root::foo()",
      "short_name": "foo",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "2:16-2:19",
          "extent": "2:3-2:21",
          "content": "virtual void foo()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [1],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 6666242542855173890,
      "detailed_name": "void Derived::foo()",
      "short_name": "foo",
      "kind": 16,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:8-5:11",
      "definition_extent": "5:3-5:25",
      "declaring_type": 1,
      "base": [0],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
