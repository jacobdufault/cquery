template<class T>
class function;

template<typename T, typename... Args>
class function<T(Args...)> {};

function<void(int)> f;

template<typename T> class allocator;

template<typename T, typename Alloc = allocator<T> >
class vector {
  void clear();
};

template<typename T>
class vector<T*> {};

struct Z1 {};

template class vector<Z1>;

struct Z2 {};

template<>
class vector<Z2> {
  void clear();
};

vector<char> vc;
vector<int*> vip;
vector<Z1> vz1;
vector<Z2> vz2;

enum Enum {
  Enum0, Enum1
};
template <typename T, int I, Enum, int E>
void foo(T Value) {}

static const int kOnst = 7;
template <>
void foo<float, 9, Enum0, kOnst + 7>(float Value);

/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 15019211479263750068,
      "detailed_name": "function",
      "short_name": "function",
      "kind": 7,
      "definition_spelling": "2:7-2:15",
      "definition_extent": "1:1-2:15",
      "parents": [],
      "derived": [1],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["2:7-2:15|-1|1|4", "5:7-5:15|-1|1|4", "7:1-7:9|-1|1|4"]
    }, {
      "id": 1,
      "usr": 10470724750229934838,
      "detailed_name": "function",
      "short_name": "function",
      "kind": 7,
      "definition_spelling": "5:7-5:15",
      "definition_extent": "4:1-5:30",
      "parents": [0],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["7:1-7:9|-1|1|4"]
    }, {
      "id": 2,
      "usr": 10862637711685426953,
      "detailed_name": "T",
      "short_name": "T",
      "kind": 25,
      "definition_spelling": "4:19-4:20",
      "definition_extent": "4:10-4:20",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["5:16-5:17|-1|1|4"]
    }, {
      "id": 3,
      "usr": 756188769017350739,
      "detailed_name": "Args",
      "short_name": "Args",
      "kind": 25,
      "definition_spelling": "4:34-4:38",
      "definition_extent": "4:22-4:38",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["5:18-5:22|-1|1|4"]
    }, {
      "id": 4,
      "usr": 15695704394170757108,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["9:28-9:37|-1|1|4", "11:39-11:48|-1|1|4"]
    }, {
      "id": 5,
      "usr": 7440942986741176606,
      "detailed_name": "vector",
      "short_name": "vector",
      "kind": 7,
      "definition_spelling": "12:7-12:13",
      "definition_extent": "12:1-14:2",
      "parents": [],
      "derived": [6, 10],
      "types": [],
      "funcs": [0],
      "vars": [],
      "instances": [1, 3, 4],
      "uses": ["17:7-17:13|-1|1|4", "26:7-26:13|-1|1|4", "30:1-30:7|-1|1|4", "31:1-31:7|-1|1|4", "32:1-32:7|-1|1|4", "33:1-33:7|-1|1|4"]
    }, {
      "id": 6,
      "usr": 16155717907537731864,
      "detailed_name": "vector",
      "short_name": "vector",
      "kind": 7,
      "definition_spelling": "17:7-17:13",
      "definition_extent": "16:1-17:20",
      "parents": [5],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [2],
      "uses": ["31:1-31:7|-1|1|4"]
    }, {
      "id": 7,
      "usr": 3421332160420436276,
      "detailed_name": "T",
      "short_name": "T",
      "kind": 25,
      "definition_spelling": "16:19-16:20",
      "definition_extent": "16:10-16:20",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["17:14-17:15|-1|1|4"]
    }, {
      "id": 8,
      "usr": 5760043510674081814,
      "detailed_name": "Z1",
      "short_name": "Z1",
      "kind": 6,
      "definition_spelling": "19:8-19:10",
      "definition_extent": "19:1-19:13",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["32:8-32:10|-1|1|4"]
    }, {
      "id": 9,
      "usr": 10124869160135436852,
      "detailed_name": "Z2",
      "short_name": "Z2",
      "kind": 6,
      "definition_spelling": "23:8-23:10",
      "definition_extent": "23:1-23:13",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["26:14-26:16|-1|1|4", "33:8-33:10|-1|1|4"]
    }, {
      "id": 10,
      "usr": 1663022413889915338,
      "detailed_name": "vector",
      "short_name": "vector",
      "kind": 7,
      "definition_spelling": "26:7-26:13",
      "definition_extent": "25:1-28:2",
      "parents": [5],
      "derived": [],
      "types": [],
      "funcs": [1],
      "vars": [],
      "instances": [],
      "uses": []
    }, {
      "id": 11,
      "usr": 9201299975592934124,
      "detailed_name": "Enum",
      "short_name": "Enum",
      "kind": 5,
      "definition_spelling": "35:6-35:10",
      "definition_extent": "35:1-37:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [5, 6],
      "instances": [],
      "uses": []
    }, {
      "id": 12,
      "usr": 2461355892344618654,
      "detailed_name": "T",
      "short_name": "T",
      "kind": 25,
      "definition_spelling": "38:20-38:21",
      "definition_extent": "38:11-38:21",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [],
      "uses": ["39:10-39:11|-1|1|4"]
    }, {
      "id": 13,
      "usr": 17,
      "detailed_name": "",
      "short_name": "",
      "kind": 0,
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [8],
      "uses": []
    }],
  "funcs": [{
      "id": 0,
      "usr": 18107614608385228556,
      "detailed_name": "void vector::clear()",
      "short_name": "clear",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "13:8-13:13",
          "extent": "13:3-13:15",
          "content": "void clear()",
          "param_spellings": []
        }],
      "declaring_type": 5,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 1,
      "usr": 6113470698424012876,
      "detailed_name": "void vector::clear()",
      "short_name": "clear",
      "kind": 16,
      "storage": 1,
      "declarations": [{
          "spelling": "27:8-27:13",
          "extent": "27:3-27:15",
          "content": "void clear()",
          "param_spellings": []
        }],
      "declaring_type": 10,
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }, {
      "id": 2,
      "usr": 17498190318698490707,
      "detailed_name": "void foo(T Value)",
      "short_name": "foo",
      "kind": 12,
      "storage": 1,
      "declarations": [{
          "spelling": "43:6-43:9",
          "extent": "42:1-43:50",
          "content": "template <>\nvoid foo<float, 9, Enum0, kOnst + 7>(float Value)",
          "param_spellings": ["43:44-43:49"]
        }],
      "definition_spelling": "39:6-39:9",
      "definition_extent": "39:1-39:21",
      "base": [],
      "derived": [],
      "locals": [],
      "uses": [],
      "callees": []
    }],
  "vars": [{
      "id": 0,
      "usr": 2933643612409209903,
      "detailed_name": "function<void (int)> f",
      "short_name": "f",
      "declarations": [],
      "definition_spelling": "7:21-7:22",
      "definition_extent": "7:1-7:22",
      "variable_type": 1,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 1,
      "usr": 5792869548777559988,
      "detailed_name": "vector<char> vc",
      "short_name": "vc",
      "declarations": [],
      "definition_spelling": "30:14-30:16",
      "definition_extent": "30:1-30:16",
      "variable_type": 5,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 2,
      "usr": 86949563628772958,
      "detailed_name": "vector<int *> vip",
      "short_name": "vip",
      "declarations": [],
      "definition_spelling": "31:14-31:17",
      "definition_extent": "31:1-31:17",
      "variable_type": 6,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 3,
      "usr": 3566687051827176322,
      "detailed_name": "vector<Z1> vz1",
      "short_name": "vz1",
      "declarations": [],
      "definition_spelling": "32:12-32:15",
      "definition_extent": "32:1-32:15",
      "variable_type": 5,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 4,
      "usr": 15931696253641284761,
      "detailed_name": "vector<Z2> vz2",
      "short_name": "vz2",
      "declarations": [],
      "definition_spelling": "33:12-33:15",
      "definition_extent": "33:1-33:15",
      "variable_type": 5,
      "uses": [],
      "parent_kind": 0,
      "kind": 13,
      "storage": 1
    }, {
      "id": 5,
      "usr": 15477793821005285152,
      "detailed_name": "Enum::Enum0",
      "short_name": "Enum0",
      "hover": "Enum::Enum0 = 0",
      "declarations": [],
      "definition_spelling": "36:3-36:8",
      "definition_extent": "36:3-36:8",
      "variable_type": 11,
      "uses": ["43:20-43:25|-1|1|4"],
      "parent_id": 11,
      "parent_kind": 2,
      "kind": 15,
      "storage": 0
    }, {
      "id": 6,
      "usr": 4917621020431490070,
      "detailed_name": "Enum::Enum1",
      "short_name": "Enum1",
      "hover": "Enum::Enum1 = 1",
      "declarations": [],
      "definition_spelling": "36:10-36:15",
      "definition_extent": "36:10-36:15",
      "variable_type": 11,
      "uses": [],
      "parent_id": 11,
      "parent_kind": 2,
      "kind": 15,
      "storage": 0
    }, {
      "id": 7,
      "usr": 10307767688451422448,
      "detailed_name": "T Value",
      "short_name": "Value",
      "declarations": [],
      "definition_spelling": "39:12-39:17",
      "definition_extent": "39:10-39:17",
      "uses": [],
      "parent_id": 2,
      "parent_kind": 3,
      "kind": 25,
      "storage": 1
    }, {
      "id": 8,
      "usr": 13914496963221806870,
      "detailed_name": "const int kOnst",
      "short_name": "kOnst",
      "hover": "const int kOnst = 7",
      "declarations": [],
      "definition_spelling": "41:18-41:23",
      "definition_extent": "41:1-41:27",
      "variable_type": 13,
      "uses": ["43:27-43:32|-1|1|4"],
      "parent_kind": 0,
      "kind": 13,
      "storage": 3
    }]
}
*/
