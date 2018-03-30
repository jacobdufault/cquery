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
      "kind": 5,
      "declarations": [],
      "spell": "7:17-7:23|-1|1|2",
      "extent": "7:1-10:2|-1|1|0",
      "bases": [],
      "derived": [],
      "types": [],
      "funcs": [0, 1, 2, 3],
      "vars": [1],
      "instances": [2],
      "uses": ["14:3-14:9|-1|1|4", "14:23-14:29|-1|1|4"]
    }, {
      "id": 1,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "declarations": [],
      "bases": [],
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
      "kind": 254,
      "storage": 0,
      "declarations": [{
          "spell": "2:11-2:15|0|2|1",
          "param_spellings": []
        }],
      "spell": "8:9-8:13|0|2|2",
      "extent": "8:1-8:16|0|2|0",
      "declaring_type": 0,
      "bases": [],
      "derived": [],
      "vars": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 4096877434426330804,
      "detailed_name": "AClass::anInstanceMethod",
      "short_name": "anInstanceMethod",
      "kind": 6,
      "storage": 0,
      "declarations": [{
          "spell": "3:11-3:27|0|2|1",
          "param_spellings": []
        }],
      "spell": "9:9-9:25|0|2|2",
      "extent": "9:1-9:28|0|2|0",
      "declaring_type": 0,
      "bases": [],
      "derived": [],
      "vars": [],
      "uses": ["15:13-15:29|4|3|32"],
      "callees": []
    }, {
      "id": 2,
      "usr": 12774569141855220778,
      "detailed_name": "AClass::aProp",
      "short_name": "aProp",
      "kind": 6,
      "storage": 0,
      "declarations": [{
          "spell": "0:0-0:0|0|2|1",
          "param_spellings": []
        }],
      "extent": "4:29-4:34|0|2|0",
      "declaring_type": 0,
      "bases": [],
      "derived": [],
      "vars": [],
      "uses": [],
      "callees": []
    }, {
      "id": 3,
      "usr": 17992064398538597892,
      "detailed_name": "AClass::setAProp:",
      "short_name": "setAProp:",
      "kind": 6,
      "storage": 0,
      "declarations": [{
          "spell": "0:0-0:0|0|2|1",
          "param_spellings": ["4:29-4:34"]
        }],
      "extent": "4:29-4:34|0|2|0",
      "declaring_type": 0,
      "bases": [],
      "derived": [],
      "vars": [],
      "uses": ["0:0-0:0|4|3|32"],
      "callees": []
    }, {
      "id": 4,
      "usr": 7033269674615638282,
      "detailed_name": "int main()",
      "short_name": "main",
      "kind": 12,
      "storage": 1,
      "declarations": [],
      "spell": "12:5-12:9|-1|1|2",
      "extent": "12:1-17:2|-1|1|0",
      "bases": [],
      "derived": [],
      "vars": [2],
      "uses": [],
      "callees": ["15:13-15:29|1|3|32", "0:0-0:0|3|3|32"]
    }],
  "vars": [{
      "id": 0,
      "usr": 14842397373703114213,
      "detailed_name": "int AClass::aProp",
      "short_name": "aProp",
      "declarations": ["4:29-4:34|0|2|1"],
      "type": 1,
      "uses": ["16:12-16:17|4|3|4"],
      "kind": 7,
      "storage": 0
    }, {
      "id": 1,
      "usr": 17112602610366149042,
      "detailed_name": "int AClass::_aProp",
      "short_name": "_aProp",
      "declarations": [],
      "spell": "4:29-4:34|0|2|2",
      "extent": "4:29-4:34|0|2|0",
      "type": 1,
      "uses": [],
      "kind": 8,
      "storage": 0
    }, {
      "id": 2,
      "usr": 6849095699869081177,
      "detailed_name": "AClass *instance",
      "short_name": "instance",
      "hover": "AClass *instance = [AClass init]",
      "declarations": [],
      "spell": "14:11-14:19|4|3|2",
      "extent": "14:3-14:35|4|3|0",
      "type": 0,
      "uses": ["15:4-15:12|4|3|4", "16:3-16:11|4|3|4"],
      "kind": 13,
      "storage": 1
    }]
}
*/
