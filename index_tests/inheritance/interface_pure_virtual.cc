class IFoo {
  virtual void foo() = 0;
};

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 9949214233977131946,
      "detailed_name": "IFoo",
      "short_name": "IFoo",
      "kind": 7,
      "definition_spelling": "1:7-1:11",
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
      "usr": 3277829753446788562,
      "detailed_name": "void IFoo::foo()",
      "short_name": "foo",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "2:16-2:19",
          "extent": "2:3-2:25",
          "content": "virtual void foo() = 0",
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
