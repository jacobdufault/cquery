@interface AClass
  + (void)test;
  - (void)anInstanceMethod;
  @property (nonatomic) int aProp;
@end

@implementation AClass
+ (void)test {}
- (void)anInstanceMethod {}
@end

int main(void)
{
  AClass *instance = [AClass init];
  [instance anInstanceMethod];
  instance.aProp = 12;
}

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 11832280568361305387,
      "detailed_name": "AClass",
      "short_name": "AClass",
      "kind": 7,
      "definition_spelling": "7:17-7:23",
      "definition_extent": "7:1-10:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [2],
      "uses": ["14:3-14:9", "14:23-14:29"]
    }, {
      "id": 1,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
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
      "usr": 12775970426728664910,
      "detailed_name": "AClass::test",
      "short_name": "test",
      "kind": 17,
      "storage": 0,
      "declarations": [{
          "spelling": "2:11-2:15",
          "extent": "2:3-2:16",
          "content": "+ (void)test;",
          "param_spellings": []
        }],
      "definition_spelling": "8:9-8:13",
      "definition_extent": "8:1-8:16",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 4096877434426330804,
      "detailed_name": "AClass::anInstanceMethod",
      "short_name": "anInstanceMethod",
      "kind": 16,
      "storage": 0,
      "declarations": [{
          "spelling": "3:11-3:27",
          "extent": "3:3-3:28",
          "content": "- (void)anInstanceMethod;",
          "param_spellings": []
        }],
      "definition_spelling": "9:9-9:25",
      "definition_extent": "9:1-9:28",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["4@15:13-15:29"],
      "callees": []
    }, {
      "id": 2,
      "usr": 12774569141855220778,
      "detailed_name": "AClass::aProp",
      "short_name": "aProp",
      "kind": 16,
      "storage": 0,
      "declarations": [{
          "spelling": "0:0-0:0",
          "extent": "4:29-4:34",
          "content": "aProp",
          "param_spellings": []
        }],
      "definition_extent": "4:29-4:34",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }, {
      "id": 3,
      "usr": 17992064398538597892,
      "detailed_name": "AClass::setAProp:",
      "short_name": "setAProp:",
      "kind": 16,
      "storage": 0,
      "declarations": [{
          "spelling": "0:0-0:0",
          "extent": "4:29-4:34",
          "content": "aProp",
          "param_spellings": ["4:29-4:34"]
        }],
      "definition_extent": "4:29-4:34",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["4@0:0-0:0"],
      "callees": []
    }, {
      "id": 4,
      "usr": 7033269674615638282,
      "detailed_name": "int main()",
      "short_name": "main",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "definition_spelling": "12:5-12:9",
      "definition_extent": "12:1-17:2",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": ["1@15:13-15:29", "3@0:0-0:0"]
    }],
  "vars": [{
      "id": 0,
      "usr": 14842397373703114213,
      "detailed_name": "int AClass::aProp",
      "short_name": "aProp",
      "declarations": ["4:29-4:34"],
      "variable_type": 1,
      "uses": ["16:12-16:17"],
      "parent_kind": 0,
      "kind": 19,
      "storage": 0
    }, {
      "id": 1,
      "usr": 17112602610366149042,
      "detailed_name": "int AClass::_aProp",
      "short_name": "_aProp",
      "declarations": [],
      "definition_spelling": "4:29-4:34",
      "definition_extent": "4:29-4:34",
      "variable_type": 1,
      "uses": [],
      "parent_kind": 0,
      "kind": 14,
      "storage": 0
    }, {
      "id": 2,
      "usr": 6849095699869081177,
      "detailed_name": "AClass *instance",
      "short_name": "instance",
      "hover": "AClass *instance = [AClass init]",
      "declarations": [],
      "definition_spelling": "14:11-14:19",
      "definition_extent": "14:3-14:35",
      "variable_type": 0,
      "uses": ["15:4-15:12", "16:3-16:11"],
      "parent_id": 4,
      "parent_kind": 3,
      "kind": 13,
      "storage": 1
    }]
}
*/
