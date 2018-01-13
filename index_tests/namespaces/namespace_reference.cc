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
  "types": [],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 17328473273923617489,
      "short_name": "Accept",
      "detailed_name": "void ns::Accept(int a)",
      "kind": 12,
      "declarations": [],
      "definition_spelling": "3:8-3:14",
      "definition_extent": "3:3-3:24",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["1@7:7-7:13", "1@9:3-9:9"],
      "callees": []
    }, {
      "id": 1,
      "is_operator": false,
      "usr": 631910859630953711,
      "short_name": "Runner",
      "detailed_name": "void Runner()",
      "kind": 12,
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
      "short_name": "Foo",
      "detailed_name": "int ns::Foo",
      "definition_spelling": "2:7-2:10",
      "definition_extent": "2:3-2:10",
      "kind": 13,
      "uses": ["2:7-2:10", "7:18-7:21", "9:10-9:13"]
    }, {
      "id": 1,
      "usr": 7976909968919750794,
      "short_name": "a",
      "detailed_name": "int a",
      "definition_spelling": "3:19-3:20",
      "definition_extent": "3:15-3:20",
      "kind": 25,
      "uses": ["3:19-3:20"]
    }]
}
*/



