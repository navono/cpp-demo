//
// Created by ping-supcon on 2021/6/17.

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>

#if USE_FOLLY
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <folly/futures/Future.h>
#else
#include "threadPool.h"
#endif

int loop_size;
thread_local double dummy = 0.0;

void process(int num) {
  double x = 0;
  double sum = 0;
  for (int i = 0; i < loop_size; ++i) {
    x += 0.0001;
    sum += sin(x) / cos(x) + cos(x) * cos(x);
  }
  dummy += sum;  // prevent optimization
}

void thread_pool_verify(int thread_pool_size, int thread_counts, int sleep_time) {
#if USE_FOLLY
  auto executor = std::make_unique<folly::CPUThreadPoolExecutor>(thread_pool_size);
#else
  auto pool = std::make_unique<ThreadPool>(thread_pool_size);
#endif

  loop_size = sleep_time;

  auto then = std::chrono::steady_clock::now();
#if USE_FOLLY
  std::vector<folly::Future<folly::Unit>> futs;
  for (int i = 0; i < thread_counts; ++i) {
    futs.emplace_back(folly::via(executor.get()).then([i]() { process(i); }));
  }
  folly::collectAll(futs).get();
#else
  for (int i = 0; i < thread_counts; ++i) {
    pool->enqueue([i]() { process(i); });
  }
  pool = nullptr;
#endif

  auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - then).count();
  std::cerr << "Time: " << diff << '\n';
}