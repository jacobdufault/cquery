template <class T>
struct C {
  T x;
  void bar();
};

template <class T>
void foo() {
  C<T> d;
  d.x;  // spelling range is empty, use cursor extent for range
  d.bar();  // spelling range is empty, use cursor extent for range

  auto e = new C<T>;
  e->x;  // `x` seems not exposed by libclang
  e->bar();  // `bar` seems not exposed by libclang
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": "c:@ST>1#T@C",
      "short_name": "C",
      "detailed_name": "C",
      "kind": 7,
      "definition_spelling": "2:8-2:9",
      "definition_extent": "2:1-5:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [0],
      "instances": [1],
      "uses": ["2:8-2:9", "9:3-9:4", "13:16-13:17"]
    }, {
      "id": 1,
      "usr": "c:member_ref_in_template.cc@10",
      "short_name": "T",
      "detailed_name": "T",
      "kind": 0,
      "definition_spelling": "1:17-1:18",
      "definition_extent": "1:11-1:18",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:17-1:18", "3:3-3:4"]
    }, {
      "id": 2,
      "usr": "c:member_ref_in_template.cc@71",
      "short_name": "T",
      "detailed_name": "T",
      "kind": 0,
      "definition_spelling": "7:17-7:18",
      "definition_extent": "7:11-7:18",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["7:17-7:18", "9:5-9:6", "13:18-13:19"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": "c:@ST>1#T@C@F@bar#",
      "short_name": "bar",
      "detailed_name": "void C::bar()",
      "kind": 16,
      "declarations": [{
          "spelling": "4:8-4:11",
          "extent": "4:3-4:13",
          "content": "void bar()",
          "param_spellings": []
        }],
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@11:3-11:8"],
      "callees": []
    }, {
      "id": 1,
      "is_operator": false,
      "usr": "c:@FT@>1#Tfoo#v#",
      "short_name": "foo",
      "detailed_name": "void foo()",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "8:6-8:9",
      "definition_extent": "8:1-16:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@11:3-11:8"]
    }],
  "vars": [{
      "id": 0,
      "usr": "c:@ST>1#T@C@FI@x",
      "short_name": "x",
      "detailed_name": "T C::x",
      "definition_spelling": "3:5-3:6",
      "definition_extent": "3:3-3:6",
      "declaring_type": 0,
      "kind": 14,
      "uses": ["3:5-3:6", "10:3-10:6"]
    }, {
      "id": 1,
      "usr": "c:member_ref_in_template.cc@97@FT@>1#Tfoo#v#@d",
      "short_name": "d",
      "detailed_name": "C<T> d",
      "definition_spelling": "9:8-9:9",
      "definition_extent": "9:3-9:9",
      "variable_type": 0,
      "kind": 13,
      "uses": ["9:8-9:9", "10:3-10:4", "11:3-11:4"]
    }, {
      "id": 2,
      "usr": "c:member_ref_in_template.cc@244@FT@>1#Tfoo#v#@e",
      "short_name": "e",
      "detailed_name": "auto e",
      "definition_spelling": "13:8-13:9",
      "definition_extent": "13:3-13:20",
      "kind": 13,
      "uses": ["13:8-13:9", "14:3-14:4", "15:3-15:4"]
    }]
}
*/
