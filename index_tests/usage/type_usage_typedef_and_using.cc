struct Foo;
using Foo1 = Foo*;
typedef Foo Foo2;
using Foo3 = Foo1;
using Foo4 = int;

void accept(Foo*) {}
void accept1(Foo1*) {}
void accept2(Foo2*) {}
void accept3(Foo3*) {}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15041163540773201510,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:8-1:11|-1|1|4", "2:14-2:17|-1|1|4", "3:9-3:12|-1|1|4", "7:13-7:16|-1|1|4"]
    }, {
      "id": 1,
      "usr": 1544499294580512394,
      "detailed_name": "Foo1",
      "short_name": "Foo1",
      "kind": 11,
      "hover": "using Foo1 = Foo*",
      "definition_spelling": "2:7-2:11",
      "definition_extent": "2:1-2:18",
      "alias_of": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:7-2:11|-1|1|4", "4:14-4:18|-1|1|4", "8:14-8:18|-1|1|4"]
    }, {
      "id": 2,
      "usr": 15466821155413653804,
      "detailed_name": "Foo2",
      "short_name": "Foo2",
      "kind": 11,
      "hover": "typedef Foo Foo2",
      "definition_spelling": "3:13-3:17",
      "definition_extent": "3:1-3:17",
      "alias_of": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["3:13-3:17|-1|1|4", "9:14-9:18|-1|1|4"]
    }, {
      "id": 3,
      "usr": 17897026942631673064,
      "detailed_name": "Foo3",
      "short_name": "Foo3",
      "kind": 11,
      "hover": "using Foo3 = Foo1",
      "definition_spelling": "4:7-4:11",
      "definition_extent": "4:1-4:18",
      "alias_of": 1,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["4:7-4:11|-1|1|4", "10:14-10:18|-1|1|4"]
    }, {
      "id": 4,
      "usr": 2638219001294786365,
      "detailed_name": "Foo4",
      "short_name": "Foo4",
      "kind": 11,
      "hover": "using Foo4 = int",
      "definition_spelling": "5:7-5:11",
      "definition_extent": "5:1-5:17",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["5:7-5:11|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 9119341505144503905,
      "detailed_name": "void accept(Foo *)",
      "short_name": "accept",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "7:6-7:12",
      "definition_extent": "7:1-7:21",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 558620830317390922,
      "detailed_name": "void accept1(Foo1 *)",
      "short_name": "accept1",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "8:6-8:13",
      "definition_extent": "8:1-8:23",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 2,
      "usr": 10523262907746124479,
      "detailed_name": "void accept2(Foo2 *)",
      "short_name": "accept2",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "9:6-9:13",
      "definition_extent": "9:1-9:23",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 3,
      "usr": 14986366321326974406,
      "detailed_name": "void accept3(Foo3 *)",
      "short_name": "accept3",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "10:6-10:13",
      "definition_extent": "10:1-10:23",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
