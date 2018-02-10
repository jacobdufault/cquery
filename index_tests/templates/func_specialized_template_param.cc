template<class T>
class Template {};

struct Foo {
  void Bar(Template<double>&);
};

void Foo::Bar(Template<double>&) {}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 17107291254533526269,
      "detailed_name": "Template",
      "short_name": "Template",
      "kind": 7,
      "definition_spelling": "2:7-2:15",
      "definition_extent": "2:1-2:18",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["5:12-5:20|-1|1|4", "8:15-8:23|-1|1|4"]
    }, {
      "id": 1,
      "usr": 15041163540773201510,
      "detailed_name": "Foo",
      "short_name": "Foo",
      "kind": 6,
      "definition_spelling": "4:8-4:11",
      "definition_extent": "4:1-6:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["8:6-8:9|-1|1|4"]
    }],
  "funcs": [{
      "id": 0,
      "usr": 8412238651648388423,
      "detailed_name": "void Foo::Bar(Template<double> &)",
      "short_name": "Bar",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "5:8-5:11",
          "extent": "5:3-5:30",
          "content": "void Bar(Template<double>&)",
          "param_spellings": ["5:29-5:29"]
        }],
      "definition_spelling": "8:11-8:14",
      "definition_extent": "8:1-8:36",
      "declaring_type": 1,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": []
}
*/
