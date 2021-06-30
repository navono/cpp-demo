#include "foo.h"

#include <interface.h>

#include <iostream>

using namespace std;

foo::foo() : queue_(1000){};

foo::~foo() = default;

void foo::hello() { std::cout << "hello from foo.dll" << std::endl; }

folly::SemiFuture<int> foo::get_fut() {
  promise_.setValue(100);
  return promise_.getSemiFuture();
}
bool foo::set_queue(folly::DMPSCQueue<int, false>& queue) {
  //  queue_ = std::move(queue);
  //  queue.set_queue(queue_);
  //  set_queue(queue)
  //  queue_(std::move(queue));

  queue.try_enqueue(200);
  return true;
}

// std::unique_ptr<folly::DMPSCQueue<int, false>> foo::get_queue() {
//  //  folly::DMPSCQueue<int, false> queue(1000);
//  return queue_;
//}

IModule* DYNALO_CALL CreateFoo() { return new foo(); }
