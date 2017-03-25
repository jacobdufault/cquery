# Naming Convention

Follow existing style, but generally:

```cc
struct MyStruct {
  void MyMethod(MyStruct* a, const std::string& b);
  int some_value;

 private:
  MyStruct* rest_;
};
```

# Header include order

Headers should go from most local to most global to expose any accidental
dependency issues. For example,

```cc
// Associated header file with the cc file.
#include "foo.h"

// Local includes.
#include "platform.h"
#include "query.h"

// Third party libraries.
#include <doctest/doctest.h>
#include <rapidjson/rapidjson.h>

// STL
#include <string>
#include <vector>
```