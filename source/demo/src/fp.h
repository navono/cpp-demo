#ifndef DEMO_FP_H
#define DEMO_FP_H

/**
 * 此样例集中在函数式编程的三个方面：
 * 1. 高阶函数（Higher-Order Functions）
 * 2. 惰性求值（On-Demand evaluation of infinite sequences）
 * 3. 组合（straightforward composition）
 * */

#include <iostream>
#include <vector>
#include <range/v3/all.hpp>
#include <range/v3/view/zip_with.hpp>

using namespace ranges;

void hof() {
  auto const v1 = std::vector{0, 2, 1};
  auto const v2 = std::vector{1, 5, 7};

  auto rng = ranges::views::zip_with(std::plus{}, v1, v2);
  std::cout << rng << std::endl;

  auto rng2 = ranges::views::zip_with([](int x, int y) { return x + y; }, v1, v2);
  std::cout << rng2 << std::endl;

  //  auto rng3 = ranges::views::iota(1) | ranges::views::take(9);

  auto stlVec = std::vector{1, 2, 3, 4, 5};  // a container - O(n) copy
  std::cout << "container: " << views::all(stlVec) << std::endl;

  auto rng4 = ranges::views::closed_iota(1, 5);           // a view - O(1) copy
  auto viewsVec = rng4 | ranges::to<std::vector<int>>();  // materialization of a view
  std::cout << "views: " << views::all(viewsVec) << std::endl;
}

void algo_views_actions() {
  /**
   *            Lazy        Composition     Modification
   * Algo        no             no              yes
   * views       yes            yes             no
   * actions     no             yes             yes
   * */

  auto v = std::vector{1, 3, 6};
  auto val = accumulate(v, 0);                                   // a range algorithm
  v |= actions::transform([](int x) { return 2 * x; });          // an action
  auto rng = v | views::transform([](int x) { return 2 * x; });  // a view
}

void views_compose() {
  // this manual
  auto r_int = views::iota(1);            // [1,2,3...]
  auto r_odd = r_int | views::stride(2);  // [1,3,5...]
  auto rng = r_odd | views::take(3);      // [1,3,5]
  std::cout << "manual: " << views::all(rng) << std::endl;

  auto rng2 = views::iota(1) | views::stride(2) | views::take(3);  // compact
  std::cout << "compact: " << views::all(rng2) << std::endl;
}

void materialization() {
  auto const v = std::vector{7, 9, -2, 5};
  auto r_filt = v | views::remove_if([](int x) { return x < 0; });  // a view
  auto v2 = r_filt | to<std::vector<int>>();                        // materialization
  sort(v2);                                                         // [5,7,9], an eager range algorithm
  std::cout << "materialization: " << views::all(v2) << std::endl;
}

void capture_by_value() {
  auto f_join = [](char c) { return [=](auto &&r) { return r | views::join(c) | to<std::string>(); }; };

  auto op = f_join('-');  // specialize to join using a dash
  auto v = std::vector<std::string>{"ab", "cd", "ef"};
  auto val = op(v);
  std::cout << "capture_by_value: " << val << std::endl;
}

void not_return_view_to_local_var() {
  /**
   * replace the local variable with an equivalent view or
   * initialize the variable in the capture list of a lambda expression
   * */

  //  auto f1_gen = []() {
  //    auto const v = std::vector{1, 2, 3};
  //    return v | views::transform([](int x) { return 2 * x; });
  //  };  // undefined behavior

  auto f2_gen = []() {
    auto rng = views::closed_iota(1, 3);
    return rng | views::transform([](int x) { return 2 * x; });
  };
  std::cout << "not_return_view_to_local_var: " << views::all(f2_gen()) << std::endl;

  auto f3_gen = [v = std::vector{1, 2, 3}]() { return v | views::transform([](int x) { return 2 * x; }); };
  std::cout << "not_return_view_to_local_var: " << views::all(f3_gen()) << std::endl;
}

void fp_demo() {
  hof();
  algo_views_actions();
  views_compose();
  materialization();

  capture_by_value();
  not_return_view_to_local_var();
}

#endif  // DEMO_FP_H
