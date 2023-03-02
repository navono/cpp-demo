#ifndef DEMO_SORT_UNIQUE_H
#define DEMO_SORT_UNIQUE_H

#include <iostream>
#include <vector>

#include <range/v3/all.hpp>
#include <range/v3/algorithm/is_sorted.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/erase.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

/**
 * include/concepts/...
 *  contains the Concepts Portability Preprocessor, or CPP, which is a set of macros for defining and
 *  using concept checks, regardless of whether your compiler happens to support the C++20 concepts language feature or
 *  not.
 * include/meta/...
 *  contains the Meta Library, which is a set of meta-programming utilities for processing types and lists of types at
 *  compile time.
 * include/range/...
 *  contains the Range-v3 library, as described below.
 *
 * 其他的参考：https://ericniebler.github.io/range-v3/
 *
 * */

using std::cout;
using namespace ranges;

void SortUnique() {
  std::vector<int> vi{9, 4, 5, 2, 9, 1, 0, 2, 6, 7, 4, 5, 6, 5, 9, 2, 7, 1, 4, 5, 3, 8, 5, 0, 2,
                      9, 3, 7, 5, 7, 5, 5, 6, 1, 4, 3, 1, 8, 4, 0, 7, 8, 8, 2, 6, 5, 3, 4, 5};

  cout << std::boolalpha;
  cout << "vector sorted :   " << ranges::is_sorted(vi) << '\n';

  vi |= actions::sort | actions::unique;
  // prints: [0,1,2,3,4,5,6,7,8,9]
  cout << "after unique sort: " << views::all(vi) << '\n';
  cout << "vector sorted :   " << ranges::is_sorted(vi) << '\n';
}

void FilterTransform() {
  std::vector<int> const vi{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto rng =
      vi | views::filter([](int i) { return i % 2 == 0; }) | views::transform([](int i) { return std::to_string(i); });

  // prints: [2,4,6,8,10]
  cout << "filter and transform: " << rng << '\n';
}

void Comprehension() {
  auto vi =
      views::for_each(views::ints(1, 6), [](int i) { return yield_from(views::repeat_n(i, i)); }) | to<std::vector>();
  // prints: [1,2,2,3,3,3,4,4,4,4,5,5,5,5,5]
  cout << "comprehension: " << views::all(vi) << '\n';
}

void AnyAllNoneOf() {
  auto is_six = [](int i) { return i == 6; };

  std::vector<int> v{6, 2, 3, 4, 5, 6};
  cout << std::boolalpha;
  cout << "vector: " << views::all(v) << '\n';

  cout << "vector any_of is_six: " << any_of(v, is_six) << '\n';
  cout << "vector all_of is_six: " << all_of(v, is_six) << '\n';
  cout << "vector none_of is_six: " << none_of(v, is_six) << '\n';
}

void Accumulate() {
  using namespace ranges;
  int sum =
      accumulate(views::ints(1, unreachable) | views::transform([](int i) { return i * i; }) | views::take(10), 0);
  // prints: 385
  cout << "sum: " << sum << '\n';
}

void TrimString() {
  std::string s = " this   ";

  // version1
  //  auto viewWord{s | views::drop_while(isspace) | views::reverse | views::drop_while(isspace) | views::reverse};
  //  cout << std::string(begin(viewWord), end(viewWord));

  // version2
  auto wsfront = ranges::find_if_not(s, isspace);
  auto wsback = ranges::find_if_not(s | views::reverse, isspace).base();
  auto result = wsback <= wsfront ? std::string() : std::string(wsfront, wsback);

  cout << "after trim blank space: " << result << "\n";
}

struct MyData {
  double value;
};
void RemoveMarkedElements() {
  std::vector<int> remove_flags = {0, 1, 1, 0, 0, 1, 1, 0};
  std::vector<MyData> rawData = {{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}};

  auto z = ranges::views::zip(remove_flags, rawData);
  auto e = ranges::remove_if(z, [](auto &&r) { return r.first; });

  //  ranges::for_each(rawData, [](auto &&d) { cout << d.value << "\n"; });
  rawData.erase(rawData.begin() + (e - z.begin()), rawData.end());
  //  remove_flags.erase(remove_flags.begin() + (e - z.begin()), remove_flags.end());
  ranges::copy(rawData | ranges::views::transform(&MyData::value), ranges::ostream_iterator(std::cout, " "));
}

void RangesV3Test() {
  SortUnique();
  FilterTransform();
  Comprehension();
  AnyAllNoneOf();
  Accumulate();
  TrimString();
  RemoveMarkedElements();
}
#endif  // DEMO_SORT_UNIQUE_H
