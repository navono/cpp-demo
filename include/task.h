#pragma once

#include <quill/Quill.h>

class Task {
 public:
  Task();

  void basic();

 private:
  quill::Logger* logger_;
};
