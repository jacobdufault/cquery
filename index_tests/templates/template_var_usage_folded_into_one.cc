enum A {};
enum B {};

template<typename T>
T var = T();

A a = var<A>;
B b = var<B>;

// NOTE: libclang before 4.0 doesn't expose template usage on |var|.

#if false
EnumDecl A
EnumDecl B
UnexposedDecl var
VarDecl a
  UnexposedExpr var
    UnexposedExpr var
      DeclRefExpr var
        TypeRef enum A
UnexposedDecl var
VarDecl b
  UnexposedExpr var
    UnexposedExpr var
      DeclRefExpr var
        TypeRef enum B
UnexposedDecl var
#endif

/*
EXTRA_FLAGS:
-std=c++14

OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": ["12:1-28:7"],
  "types": [{
      "id": 0,
      "usr": 6697181287623958829,
      "short_name": "A",
      "detailed_name": "A",
      "kind": 5,
      "definition_spelling": "1:6-1:7",
      "definition_extent": "1:1-1:10",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [1],
      "uses": ["1:6-1:7", "7:1-7:2", "7:11-7:12"]
    }, {
      "id": 1,
      "usr": 13892793056005362145,
      "short_name": "B",
      "detailed_name": "B",
      "kind": 5,
      "definition_spelling": "2:6-2:7",
      "definition_extent": "2:1-2:10",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [2],
      "uses": ["2:6-2:7", "8:1-8:2", "8:11-8:12"]
    }, {
      "id": 2,
      "usr": 8864163146308556810,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["5:1-5:2", "5:9-5:10"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 8096973118640070624,
      "short_name": "var",
      "detailed_name": "T var",
      "definition_spelling": "5:3-5:6",
      "definition_extent": "5:1-5:12",
      "kind": 13,
      "uses": ["5:3-5:6", "7:7-7:10", "8:7-8:10"]
    }, {
      "id": 1,
      "usr": 16721564935990383768,
      "short_name": "a",
      "detailed_name": "A a",
      "definition_spelling": "7:3-7:4",
      "definition_extent": "7:1-7:13",
      "variable_type": 0,
      "kind": 13,
      "uses": ["7:3-7:4"]
    }, {
      "id": 2,
      "usr": 12028309045033782423,
      "short_name": "b",
      "detailed_name": "B b",
      "definition_spelling": "8:3-8:4",
      "definition_extent": "8:1-8:13",
      "variable_type": 1,
      "kind": 13,
      "uses": ["8:3-8:4"]
    }]
}
*/
