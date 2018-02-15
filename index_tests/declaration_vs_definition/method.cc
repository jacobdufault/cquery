class Foo {
  void declonly();
  virtual void purevirtual() = 0;
  void def();
};

void Foo::def() {}

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
      "kind": 7,
      "spell": "1:7-1:10|-1|1|2",
      "extent": "1:1-5:2|-1|1|0",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0, 1, 2],
      "vars": [],
      "instances": [],
      "uses": ["7:6-7:9|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 4012226004228259562,
      "detailed_name": "void Foo::declonly()",
      "short_name": "declonly",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "2:8-2:16",
          "extent": "2:3-2:18",
          "content": "void declonly()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 10939323144126021546,
      "detailed_name": "void Foo::purevirtual()",
      "short_name": "purevirtual",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "3:16-3:27",
          "extent": "3:3-3:33",
          "content": "virtual void purevirtual() = 0",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 2,
      "usr": 15416083548883122431,
      "detailed_name": "void Foo::def()",
      "short_name": "def",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "4:8-4:11",
          "extent": "4:3-4:13",
          "content": "void def()",
          "param_spellings": []
        }],
      "spell": "7:11-7:14|0|2|2",
      "extent": "7:1-7:19|-1|1|0",
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
