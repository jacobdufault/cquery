namespace ns {
  int Foo;
  void Accept(int a) {}
}

void Runner() {
  ns::Accept(ns::Foo);
  using namespace ns;
  Accept(Foo);
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 11072669167287398027,
      "detailed_name": "ns",
      "short_name_offset": 0,
      "short_name_size": 2,
      "kind": 2,
      "definition_spelling": "1:11-1:13",
      "definition_extent": "1:1-4:2",
      "parents": [1],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [0],
      "instances": [],
      "uses": ["1:11-1:13", "7:3-7:5", "7:14-7:16", "8:19-8:21"]
    }, {
      "id": 1,
      "usr": 13838176792705659279,
      "detailed_name": "<fundamental>",
      "short_name_offset": 0,
      "short_name_size": 0,
      "kind": 0,
      "parents": [],
      "derived": [0],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": []
    }, {
      "id": 2,
      "usr": 17,
      "detailed_name": "",
      "short_name_offset": 0,
      "short_name_size": 0,
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
      "usr": 17328473273923617489,
      "detailed_name": "void ns::Accept(int a)",
      "short_name_offset": 9,
      "short_name_size": 6,
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "3:8-3:14",
      "definition_extent": "3:3-3:24",
      "declaring_type": 0,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@7:7-7:13", "1@9:3-9:9"],
      "callees": []
    }, {
      "id": 1,
      "usr": 631910859630953711,
      "detailed_name": "void Runner()",
      "short_name_offset": 5,
      "short_name_size": 6,
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "6:6-6:12",
      "definition_extent": "6:1-10:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@7:7-7:13", "0@9:3-9:9"]
    }],
  "vars": [{
      "id": 0,
      "usr": 12898699035586282159,
      "detailed_name": "int ns::Foo",
      "short_name_offset": 8,
      "short_name_size": 3,
      "declarations": [],
      "definition_spelling": "2:7-2:10",
      "definition_extent": "2:3-2:10",
      "variable_type": 2,
      "uses": ["7:18-7:21", "9:10-9:13"],
      "parent_id": 0,
      "parent_kind": 2,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 7976909968919750794,
      "detailed_name": "int a",
      "short_name_offset": 4,
      "short_name_size": 1,
      "declarations": [],
      "definition_spelling": "3:19-3:20",
      "definition_extent": "3:15-3:20",
      "variable_type": 2,
      "uses": [],
      "parent_id": 0,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }]
}
*/



