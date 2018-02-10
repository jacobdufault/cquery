class Foo;
class Foo;
class Foo {};
class Foo;

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
      "definition_spelling": "3:7-3:10",
      "definition_extent": "3:1-3:13",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:7-1:10|-1|1|4", "2:7-2:10|-1|1|4", "4:7-4:10|-1|1|4"]
    }],
  "funcs": [],
  "vars": []
}
*/
