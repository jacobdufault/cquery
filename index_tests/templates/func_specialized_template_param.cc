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
      "short_name": "Template",
      "detailed_name": "Template",
      "kind": 7,
      "definition_spelling": "2:7-2:15",
      "definition_extent": "2:1-2:18",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:7-2:15", "5:12-5:20", "8:15-8:23"]
    }, {
      "id": 1,
      "usr": 15041163540773201510,
      "short_name": "Foo",
      "detailed_name": "Foo",
      "kind": 6,
      "definition_spelling": "4:8-4:11",
      "definition_extent": "4:1-6:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [],
      "uses": ["4:8-4:11", "8:6-8:9"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 8412238651648388423,
      "short_name": "Bar",
      "detailed_name": "void Foo::Bar(Template<double> &)",
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
      "callers": [],
      "callees": []
    }],
  "vars": []
}
*/
