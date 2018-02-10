void called(int a);

int gen();

struct Foo {
  static int static_var;
  int field_var;
};

int Foo::static_var = 0;

void foo() {
  int a = 5;
  called(a + gen() + Foo().field_var + Foo::static_var);
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
      "definition_spelling": "5:8-5:11",
      "definition_extent": "5:1-8:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [1, 0],
      "instances": [],
      "uses": ["10:5-10:8|-1|1|4", "14:22-14:25|-1|1|4", "14:40-14:43|-1|1|4"]
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
      "instances": [0, 1, 2],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 18319417758892371313,
      "detailed_name": "void called(int a)",
      "short_name": "called",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "1:6-1:12",
          "extent": "1:1-1:19",
          "content": "void called(int a)",
          "param_spellings": ["1:17-1:18"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["14:3-14:9|2|3|64"],
      "callees": []
    }, {
      "id": 1,
      "usr": 11404602816585117695,
      "detailed_name": "int gen()",
      "short_name": "gen",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "3:5-3:8",
          "extent": "3:1-3:10",
          "content": "int gen()",
          "param_spellings": []
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": ["14:14-14:17|2|3|64"],
      "callees": []
    }, {
      "id": 2,
      "usr": 4259594751088586730,
      "detailed_name": "void foo()",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "12:6-12:9",
      "definition_extent": "12:1-15:2",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": ["14:3-14:9|0|3|64", "14:14-14:17|1|3|64"]
    }],
  "vars": [{
      "id": 0,
      "usr": 11489549839875479478,
      "detailed_name": "int Foo::static_var",
      "short_name": "static_var",
      "hover": "int Foo::static_var = 0",
      "declarations": ["6:14-6:24"],
      "definition_spelling": "10:10-10:20",
      "definition_extent": "10:1-10:24",
      "variable_type": 1,
      "uses": ["14:45-14:55|2|3|4"],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 21,
      "storage": 1
    }, {
      "id": 1,
      "usr": 9648311402855509901,
      "detailed_name": "int Foo::field_var",
      "short_name": "field_var",
      "declarations": [],
      "definition_spelling": "7:7-7:16",
      "definition_extent": "7:3-7:16",
      "variable_type": 1,
      "uses": ["14:28-14:37|2|3|4"],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 14,
      "storage": 0
    }, {
      "id": 2,
      "usr": 13284113377394221067,
      "detailed_name": "int a",
      "short_name": "a",
      "hover": "int a = 5",
      "declarations": [],
      "definition_spelling": "13:7-13:8",
      "definition_extent": "13:3-13:12",
      "variable_type": 1,
      "uses": ["14:10-14:11|2|3|4"],
      "parent_id": 2,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
