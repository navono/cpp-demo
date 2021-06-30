#include "task.h"

#include <taskflow/taskflow.hpp>

Task::Task() { logger_ = quill::get_logger("app"); }

void Task::basic() {
  tf::Executor executor;
  tf::Taskflow taskflow;

  auto [A, B, C, D] = taskflow.emplace(
      // create task
      [&]() { LOG_INFO(logger_, "task A"); }, [&]() { LOG_INFO(logger_, "task B"); },
      [&]() { LOG_INFO(logger_, "task C"); }, [&]() { LOG_INFO(logger_, "task D"); });

  A.precede(B, C);  // A runs before B and C
  D.succeed(B, C);  // D runs after  B and C

  executor.run(taskflow).wait();
}
