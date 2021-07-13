#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <utility>

// void f(int& x) {
//  spdlog::info(fmt::format("f(int& x) with {}", x));
//}
//
// void f(int&& x) {
//  spdlog::info(fmt::format("f(int&& x) with {}", x));
//}

void f(auto&& x) {
  //  T& & becomes T&
  //  T& && becomes T&
  //  T&& & becomes T&
  //  T&& && becomes T&&

  spdlog::info(fmt::format("f(auto&& x) with {}", x));
}

void testRvalue() {
  int x = 10;      // `x` is an lvalue of type `int`
  int& xl = x;     // `xl` is an lvalue of type `int&`
                   // int&& xr = x; // compiler error -- `x` is an lvalue
  int&& xr2 = 20;  // `xr2` is an lvalue of type `int&&` -- binds to the rvalue temporary, `0`

  //  f(x);   // calls f(int&)
  //  f(xl);  // calls f(int&)
  //  f(3);   // calls f(int&&)
  //  f(std::move(x));  // calls f(int&&)

  f(static_cast<decltype(x)&&>(x));  // same as f(std::move(x))
  spdlog::info("x after std::move. {}", x);

  f(xr2);             // calls f(int&)
  f(std::move(xr2));  // calls f(int&& x)
}
