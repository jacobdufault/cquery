struct Foo {
  void Used();
};

void user() {
  Foo* f = nullptr;
  f->Used();
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
      "spell": "1:8-1:11|-1|1|2",
      "extent": "1:1-3:2|-1|1|0",
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
      "usr": 18417145003926999463,
      "detailed_name": "void Foo::Used()",
      "short_name": "Used",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "2:8-2:12",
          "extent": "2:3-2:14",
          "content": "void Used()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["7:6-7:10|1|3|32"],
      "callees": []
    }, {
      "id": 1,
      "usr": 9376923949268137283,
      "detailed_name": "void user()",
      "short_name": "user",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "spell": "5:6-5:10|-1|1|2",
      "extent": "5:1-8:2|-1|1|0",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["7:6-7:10|0|3|32"]
    }],
  "vars": [{
      "id": 0,
      "usr": 3014406561587537195,
      "detailed_name": "Foo *f",
      "short_name": "f",
      "hover": "Foo *f = nullptr",
      "declarations": [],
      "spell": "6:8-6:9|1|3|2",
      "extent": "6:3-6:19|1|3|2",
      "type": 0,
      "uses": ["7:3-7:4|1|3|4"],
      "kind": 13,
      "storage": 1
    }]
}
*/
