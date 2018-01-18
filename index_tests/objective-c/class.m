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
      "short_name": "AClass",
      "detailed_name": "AClass",
      "kind": 7,
      "definition_spelling": "7:17-7:23",
      "definition_extent": "7:1-10:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["1:12-1:18", "7:17-7:23", "14:3-14:9", "14:23-14:29"]
    }],
  "funcs": [{
      "id": 0,
      "is_operator": false,
      "usr": 12775970426728664910,
      "short_name": "test",
      "detailed_name": "AClass::test",
      "kind": 17,
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
      "is_operator": false,
      "usr": 4096877434426330804,
      "short_name": "anInstanceMethod",
      "detailed_name": "AClass::anInstanceMethod",
      "kind": 16,
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
      "is_operator": false,
      "usr": 12774569141855220778,
      "short_name": "aProp",
      "detailed_name": "AClass::aProp",
      "kind": 16,
      "declarations": [{
          "spelling": "0:0-0:0",
          "extent": "4:29-4:34",
          "content": "aProp",
          "param_spellings": []
        }],
      "definition_spelling": "0:0-0:0",
      "definition_extent": "4:29-4:34",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": [],
      "callees": []
    }, {
      "id": 3,
      "is_operator": false,
      "usr": 17992064398538597892,
      "short_name": "setAProp:",
      "detailed_name": "AClass::setAProp:",
      "kind": 16,
      "declarations": [{
          "spelling": "0:0-0:0",
          "extent": "4:29-4:34",
          "content": "aProp",
          "param_spellings": ["4:29-4:34"]
        }],
      "definition_spelling": "0:0-0:0",
      "definition_extent": "4:29-4:34",
      "base": [],
      "derived": [],
      "locals": [],
      "callers": ["4@0:0-0:0"],
      "callees": []
    }, {
      "id": 4,
      "is_operator": false,
      "usr": 7033269674615638282,
      "short_name": "main",
      "detailed_name": "int main()",
      "kind": 12,
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
      "short_name": "aProp",
      "detailed_name": "int AClass::aProp",
      "declaration": "4:29-4:34",
      "kind": 19,
      "uses": ["4:29-4:34", "16:12-16:17"]
    }, {
      "id": 1,
      "usr": 17112602610366149042,
      "short_name": "_aProp",
      "detailed_name": "int AClass::_aProp",
      "definition_spelling": "4:29-4:34",
      "definition_extent": "4:29-4:34",
      "kind": 14,
      "uses": ["4:29-4:34"]
    }, {
      "id": 2,
      "usr": 6849095699869081177,
      "short_name": "instance",
      "detailed_name": "AClass *instance",
      "hover": "AClass *instance = [AClass init]",
      "definition_spelling": "14:11-14:19",
      "definition_extent": "14:3-14:35",
      "kind": 13,
      "uses": ["14:11-14:19", "15:4-15:12", "16:3-16:11"]
    }]
}
*/
