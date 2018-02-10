class Foo {
  void operator()(int) { }
  void operator()(bool);
  int operator()(int a, int b);
};

Foo &operator += (const Foo&, const int&);

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
      "funcs": [0, 1, 2],
      "vars": [],
      "instances": [],
      "uses": ["7:1-7:4|-1|1|4", "7:25-7:28|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 7874436189163837815,
      "detailed_name": "void Foo::operator()(int)",
      "short_name": "operator()",
      "kind": 16,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "2:8-2:18",
      "definition_extent": "2:3-2:27",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 3545323327609582678,
      "detailed_name": "void Foo::operator()(bool)",
      "short_name": "operator()",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "3:8-3:18",
          "extent": "3:3-3:24",
          "content": "void operator()(bool)",
          "param_spellings": ["3:23-3:23"]
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 2,
      "usr": 3986818119971932909,
      "detailed_name": "int Foo::operator()(int a, int b)",
      "short_name": "operator()",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "4:7-4:17",
          "extent": "4:3-4:31",
          "content": "int operator()(int a, int b)",
          "param_spellings": ["4:22-4:23", "4:29-4:30"]
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 3,
      "usr": 8288368475529136092,
      "detailed_name": "Foo &operator+=(const Foo &, const int &)",
      "short_name": "operator+=",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "7:6-7:17",
          "extent": "7:1-7:42",
          "content": "Foo &operator += (const Foo&, const int&)",
          "param_spellings": ["7:29-7:29", "7:41-7:41"]
        }],
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
