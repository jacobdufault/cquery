#include "make_functions.h"

template <typename T, typename... Args>
T* MakeUnique(Args&&... args) {
  return nullptr;
}

template <typename T, typename... Args>
T* maKE_NoRefs(Args... args) {
  return nullptr;
}

void caller22() {
  MakeUnique<Foobar>();
  MakeUnique<Foobar>(1);
  MakeUnique<Foobar>(1, new Bar(), nullptr);
  maKE_NoRefs<Foobar>(1, new Bar(), nullptr);
}

// TODO: Eliminate the extra entries in the "types" array here. They come from
// the template function definitions.

// Foobar is defined in a separate file to ensure that we can attribute
// MakeUnique calls across translation units.

/*
OUTPUT: make_functions.h
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 12993848456528750350,
      "short_name": "Bar",
      "detailed_name": "Bar",
      "kind": 6,
      "definition_spelling": "1:8-1:11",
      "definition_extent": "1:1-1:14",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:11", "7:17-7:20", "8:15-8:18"]
    }, {
      "id": 1,
      "usr": 14935975554338052500,
      "short_name": "Foobar",
      "detailed_name": "Foobar",
      "kind": 7,
      "definition_spelling": "3:7-3:13",
      "definition_extent": "3:1-9:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0, 1, 2, 3],
      "vars": [],
      "instances": [],
      "uses": ["3:7-3:13", "5:3-5:9", "6:3-6:9", "7:3-7:9", "8:3-8:9"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 13131778807733950299,
      "short_name": "Foobar",
      "detailed_name": "void Foobar::Foobar()",
      "kind": 22,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "5:3-5:9",
      "definition_extent": "5:3-5:14",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 13028995015627606181,
      "short_name": "Foobar",
      "detailed_name": "void Foobar::Foobar(int)",
      "kind": 22,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "6:3-6:9",
      "definition_extent": "6:3-6:17",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }, {
      "id": 2,
      "usr": 3765833212244435302,
      "short_name": "Foobar",
      "detailed_name": "void Foobar::Foobar(int &&, Bar *, bool *)",
      "kind": 22,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "7:3-7:9",
      "definition_extent": "7:3-7:32",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }, {
      "id": 3,
      "usr": 17321436359755983845,
      "short_name": "Foobar",
      "detailed_name": "void Foobar::Foobar(int, Bar *, bool *)",
      "kind": 22,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "8:3-8:9",
      "definition_extent": "8:3-8:30",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }],
  "vars": []
}
OUTPUT: make_functions.cc
{
  "includes": [{
      "line": 0,
      "resolved_path": "&make_functions.h"
    }],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 7902098450755788854,
      "short_name": "T",
      "detailed_name": "T",
      "kind": 0,
      "definition_spelling": "3:20-3:21",
      "definition_extent": "3:11-3:21",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["4:1-4:2", "3:20-3:21"]
    }, {
      "id": 1,
      "usr": 12533159752419999454,
      "short_name": "Args",
      "detailed_name": "Args",
      "kind": 0,
      "definition_spelling": "3:35-3:39",
      "definition_extent": "3:23-3:39",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["3:35-3:39", "4:15-4:19"]
    }, {
      "id": 2,
      "usr": 18441628706991062891,
      "short_name": "T",
      "detailed_name": "T",
      "kind": 0,
      "definition_spelling": "8:20-8:21",
      "definition_extent": "8:11-8:21",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["9:1-9:2", "8:20-8:21"]
    }, {
      "id": 3,
      "usr": 9441341235704820385,
      "short_name": "Args",
      "detailed_name": "Args",
      "kind": 0,
      "definition_spelling": "8:35-8:39",
      "definition_extent": "8:23-8:39",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["8:35-8:39", "9:16-9:20"]
    }, {
      "id": 4,
      "usr": 14935975554338052500,
      "short_name": "Foobar",
      "detailed_name": "Foobar",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["14:14-14:20", "15:14-15:20", "16:14-16:20", "17:15-17:21"]
    }, {
      "id": 5,
      "usr": 12993848456528750350,
      "short_name": "Bar",
      "detailed_name": "Bar",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["16:29-16:32", "17:30-17:33"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 15793662558620604611,
      "short_name": "MakeUnique",
      "detailed_name": "T *MakeUnique(Args &&... args)",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "4:4-4:14",
      "definition_extent": "4:1-6:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["2@14:3-14:13", "2@15:3-15:13", "2@16:3-16:13"],
      "callees": []
    }, {
      "id": 1,
      "usr": 2532818908869373467,
      "short_name": "maKE_NoRefs",
      "detailed_name": "T *maKE_NoRefs(Args... args)",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "9:4-9:15",
      "definition_extent": "9:1-11:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["2@17:3-17:14"],
      "callees": []
    }, {
      "id": 2,
      "usr": 2816883305867289955,
      "short_name": "caller22",
      "detailed_name": "void caller22()",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "13:6-13:14",
      "definition_extent": "13:1-18:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["0@14:3-14:13", "0@15:3-15:13", "0@16:3-16:13", "1@17:3-17:14"]
    }, {
      "id": 3,
      "usr": 13131778807733950299,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "storage": 0,
      "declarations": [],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["~-1@14:3-14:13"],
      "callees": []
    }, {
      "id": 4,
      "usr": 13028995015627606181,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "storage": 0,
      "declarations": [],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["~-1@15:3-15:13"],
      "callees": []
    }, {
      "id": 5,
      "usr": 3765833212244435302,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "storage": 0,
      "declarations": [],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["~-1@16:3-16:13"],
      "callees": []
    }, {
      "id": 6,
      "usr": 17321436359755983845,
      "short_name": "",
      "detailed_name": "",
      "kind": 0,
      "storage": 0,
      "declarations": [],
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["~-1@17:3-17:14"],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 15288691366352169805,
      "short_name": "args",
      "detailed_name": "Args &&... args",
      "kind": 25,
      "storage": 1,
      "definition_spelling": "4:25-4:29",
      "definition_extent": "4:15-4:29",
      "uses": ["4:25-4:29"]
    }, {
      "id": 1,
      "usr": 12338908251430965107,
      "short_name": "args",
      "detailed_name": "Args... args",
      "kind": 25,
      "storage": 1,
      "definition_spelling": "9:24-9:28",
      "definition_extent": "9:16-9:28",
      "uses": ["9:24-9:28"]
    }]
}
*/
