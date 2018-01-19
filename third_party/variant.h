#if __cplusplus < 201703L
#include "variant.hpp"
namespace std {
using mpark::holds_alternative;
using mpark::get;
using mpark::get_if;
using mpark::monostate;
using mpark::variant;
}
#else
#include <variant>
#endif
