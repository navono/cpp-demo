#pragma once

template <int N>  // (2)
struct Factorial {
  static int const value = N * Factorial<N - 1>::value;
};

template <>  // (3)
struct Factorial<1> {
  static int const value = 1;
};