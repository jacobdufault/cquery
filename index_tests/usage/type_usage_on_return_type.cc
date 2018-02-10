struct Type;

Type* foo();
Type* foo();
Type* foo() { return nullptr; }

class Foo {
  Type* Get(int);
  void Empty();
};

Type* Foo::Get(int) { return nullptr; }
void Foo::Empty() {}

extern const Type& external();

static Type* bar();
static Type* bar() { return nullptr; }

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 13487927231218873822,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:12|-1|1|4", "3:1-3:5|-1|1|4", "4:1-4:5|-1|1|4", "5:1-5:5|-1|1|4", "8:3-8:7|-1|1|4", "12:1-12:5|-1|1|4", "15:14-15:18|-1|1|4", "17:8-17:12|-1|1|4", "18:8-18:12|-1|1|4"]
    }, {
      "id": 1,
      "usr": 15041163540773201510,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 7,
      "definition_spelling": "7:7-7:10",
      "definition_extent": "7:1-10:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [1, 2],
      "vars": [],
      "instances": [],
      "uses": ["12:7-12:10|-1|1|4", "13:6-13:9|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 4259594751088586730,
      "detailed_name": "Type *foo()",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "3:7-3:10",
          "extent": "3:1-3:12",
          "content": "Type* foo()",
          "param_spellings": []
        }, {
          "spelling": "4:7-4:10",
          "extent": "4:1-4:12",
          "content": "Type* foo()",
          "param_spellings": []
        }],
      "definition_spelling": "5:7-5:10",
      "definition_extent": "5:1-5:32",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 13402221340333431092,
      "detailed_name": "Type *Foo::Get(int)",
      "short_name": "Get",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "8:9-8:12",
          "extent": "8:3-8:17",
          "content": "Type* Get(int)",
          "param_spellings": ["8:16-8:16"]
        }],
      "definition_spelling": "12:12-12:15",
      "definition_extent": "12:1-12:40",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 2,
      "usr": 4240751906910175539,
      "detailed_name": "void Foo::Empty()",
      "short_name": "Empty",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "9:8-9:13",
          "extent": "9:3-9:15",
          "content": "void Empty()",
          "param_spellings": []
        }],
      "definition_spelling": "13:11-13:16",
      "definition_extent": "13:1-13:21",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 3,
      "usr": 7746867874366499515,
      "detailed_name": "const Type &external()",
      "short_name": "external",
      "kind": 12,
      "storage": 2,
      "declarations": [{
          "spelling": "15:20-15:28",
          "extent": "15:1-15:30",
          "content": "extern const Type& external()",
          "param_spellings": []
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 4,
      "usr": 18408440185620243373,
      "detailed_name": "Type *bar()",
      "short_name": "bar",
      "kind": 12,
      "storage": 3,
      "declarations": [{
          "spelling": "17:14-17:17",
          "extent": "17:1-17:19",
          "content": "static Type* bar()",
          "param_spellings": []
        }],
      "definition_spelling": "18:14-18:17",
      "definition_extent": "18:1-18:39",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
