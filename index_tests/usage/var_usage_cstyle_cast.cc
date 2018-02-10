enum VarType {};

struct Holder {
  static constexpr VarType static_var = (VarType)0x0;
};

const VarType Holder::static_var;


/*
OUTPUT:
{
  "includes": [],
  "skipped_by_preprocessor": [],
  "types": [{
      "id": 0,
      "usr": 5792006888140599735,
      "detailed_name": "VarType",
      "short_name": "VarType",
      "kind": 5,
      "definition_spelling": "1:6-1:13",
      "definition_extent": "1:1-1:16",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [],
      "instances": [0],
      "uses": ["4:20-4:27|-1|1|4", "4:42-4:49|-1|1|4", "7:7-7:14|-1|1|4"]
    }, {
      "id": 1,
      "usr": 10028537921178202800,
      "detailed_name": "Holder",
      "short_name": "Holder",
      "kind": 6,
      "definition_spelling": "3:8-3:14",
      "definition_extent": "3:1-5:2",
      "parents": [],
      "derived": [],
      "types": [],
      "funcs": [],
      "vars": [0],
      "instances": [],
      "uses": ["7:15-7:21|-1|1|4"]
    }],
  "funcs": [],
  "vars": [{
      "id": 0,
      "usr": 7057400933868440116,
      "detailed_name": "const VarType Holder::static_var",
      "short_name": "static_var",
      "hover": "const VarType Holder::static_var = (VarType)0x0",
      "declarations": ["4:28-4:38"],
      "definition_spelling": "7:23-7:33",
      "definition_extent": "7:1-7:33",
      "variable_type": 0,
      "uses": [],
      "parent_id": 1,
      "parent_kind": 2,
      "kind": 21,
      "storage": 1
    }]
}
*/

























//#include <string>
//#include <xiosbase>

//#include <sstream>
//#include <algorithm>
//#include <vector>
//#include <string>
//#include <cstddef>
//#include <sstream>
//#include <iomanip>
//#include <limits>
//#include <vector>
//#include <cstddef>
//#include <tuple>
//#include <type_traits>
//#include <string>
//#include <string>
//#include <type_traits>
//#include <iterator>
//#include <vector>
//#include <string>
//#include <stdlib.h>
//#include <string>
//#include <vector>
//#include <string>
//#include <cstddef>
//#include <cmath>
//#include <limits>
//#include <type_traits>
//#include <set>
//#include <string>
//#include <vector>
//#include <iosfwd>
//#include <streambuf>
//#include <ostream>
//#include <fstream>
//#include <memory>
//#include <vector>
//#include <string>
//#include <stdexcept>
//#include <string>
//#include <vector>
//#include <sstream>
//#include <algorithm>
