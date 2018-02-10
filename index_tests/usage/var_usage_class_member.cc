class Foo {
public:
  int x;
  int y;
};

void accept(int);
void accept(int*);

void foo() {
  Foo f;
  f.x = 3;
  f.x += 5;
  accept(f.x);
  accept(f.x + 20);
  accept(&f.x);
  accept(f.y);
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
      "kind": 7,
      "definition_spelling": "1:7-1:10",
      "definition_extent": "1:1-5:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0, 1],
      "instances": [2],
      "uses": ["11:3-11:6|-1|1|4"]
    }, {
      "id": 1,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0, 1],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 17175780305784503374,
      "detailed_name": "void accept(int)",
      "short_name": "accept",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "7:6-7:12",
          "extent": "7:1-7:17",
          "content": "void accept(int)",
          "param_spellings": ["7:16-7:16"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["14:3-14:9|2|3|64", "15:3-15:9|2|3|64", "17:3-17:9|2|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 12086644540399881766,
      "detailed_name": "void accept(int *)",
      "short_name": "accept",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "8:6-8:12",
          "extent": "8:1-8:18",
          "content": "void accept(int*)",
          "param_spellings": ["8:17-8:17"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["16:3-16:9|2|3|64"],
      "callees": []
    }, {
      "id": 2,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "10:6-10:9",
      "definition_extent": "10:1-18:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["14:3-14:9|0|3|64", "15:3-15:9|0|3|64", "16:3-16:9|1|3|64", "17:3-17:9|0|3|64"]
    }],
  "vars": [{
      "id": 0,
      "usr": 4220150017963593039,
      "detailed_name": "int Foo::x",
      "short_name": "x",
      "declarations": [],
      "definition_spelling": "3:7-3:8",
      "definition_extent": "3:3-3:8",
      "variable_type": 1,
      "uses": ["12:5-12:6|2|3|4", "13:5-13:6|2|3|4", "14:12-14:13|2|3|4", "15:12-15:13|2|3|4", "16:13-16:14|2|3|4"],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 1,
      "usr": 3873837747174060388,
      "detailed_name": "int Foo::y",
      "short_name": "y",
      "declarations": [],
      "definition_spelling": "4:7-4:8",
      "definition_extent": "4:3-4:8",
      "variable_type": 1,
      "uses": ["17:12-17:13|2|3|4"],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 2,
      "usr": 16303259148898744165,
      "detailed_name": "Foo f",
      "short_name": "f",
      "declarations": [],
      "definition_spelling": "11:7-11:8",
      "definition_extent": "11:3-11:8",
      "variable_type": 0,
      "uses": ["12:3-12:4|2|3|4", "13:3-13:4|2|3|4", "14:10-14:11|2|3|4", "15:10-15:11|2|3|4", "16:11-16:12|2|3|4", "17:10-17:11|2|3|4"],
      "parent_id": 2,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
