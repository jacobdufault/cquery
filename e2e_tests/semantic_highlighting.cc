#include <stddef.h>
#pragma clang attribute push([[nodiscard]], apply_to=enum)
enum class E { E0, E1 = 1, E2, E3 };
#pragma clang attribute pop

#define M0(x)
#define M1(x) M0(x)+M0(x)
#define M2 __FILE__
#define M3 __LINE__
#define M4(x, y) x##y

#if __has_feature(meow) && __clang__
// skipped range
#endif

extern int extern0, extern1, extern2;
thread_local double thread0 = 42, thread1 = 1.911042e9, thread2 = 0xcafe;
int global0 = 20130802, global1 = 0xdefc0, global2 __attribute__((vector_size(16)));
static int static_global0, static_global1, static_global2;

struct A {
  unsigned int mem0, mem1, mem2;
  static int static_mem0, static_mem1, static_mem2;
  A();
  ~A();
  A&& mem_func0() noexcept;
  virtual void mem_func1() {}
  void mem_func2() __attribute__((deprecated(""))) {}
  static int static_func0();
  static void static_func1() {}
  static void static_func2() throw() {}
};

struct S0; struct S1 {}; struct S2 : A, S1 {};
class C0; class C1 {}; class C2 {};
union U0; union U1 {}; union U2 {};

template <class T0, class T1, class T2, int I0, int I1, class F, class F1>
int foo(F &&f, F1, int p = 0) {
  T0 t0 = 0xb612;
  T1 t1{46610};
  T2 t2(44);
  t0 = t1 + t2 + long(&t0) + (long long)A::static_func0;
  t2 = t1 = t0;
  __builtin_trap();
  return t0 ? t1 * t2 << I0 : I1;
}

namespace n0 {
  typedef int T0;
  typedef long double T1[sizeof(T0)];
  using T2 = T1;
  namespace n1::n2::n3::n4::n5 {
    void foo(float p0, short p1 = 555) {
      auto a = A();
      (new A)->~A();
      new char[sizeof(p0)];
      int local0 = 20140819;
      double local1 = 0x2015071f5, local2 = 20161010;
      static int static_mem0, static_mem1, static_mem2;
      extern __fp16 extern0, extern1, extern2;
      auto l0 = [&](int q0, int q1) { return q0 + q1; };
      ::foo<int, int, int, 0, 1>([p0, p1](int q0, int q1) { return p0 + q0; }, l0);
    }
  }
}

using namespace n0;
