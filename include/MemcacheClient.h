#pragma once

#include <folly/futures/Future.h>

#include <functional>
#include <string>

using std::string;
class MemcacheClient {
 public:
  MemcacheClient();

  struct GetReply {
    enum class Result {
      FOUND,
      NOT_FOUND,
      SERVER_ERROR,
    };

    Result result;
    // The value when result is FOUND,
    // The error message when result is SERVER_ERROR or CLIENT_ERROR
    // undefined otherwise
    string value;
  };

  // 同步
  GetReply get(string key);

  // 异步
  int async_get(string key, std::function<void(GetReply)> callback);

  // 基于 Future
  // Example Code
  //  SemiFuture<GetReply> semiFut = mc.future_get("foo");
  //  Future<GetReply> fut1 = std::move(semiFut).via(&executor);
  //
  //  Future<string> fut2 = std::move(fut1).thenValue(
  //      [](GetReply reply) {
  //        if (reply.result == MemcacheClient::GetReply::Result::FOUND)
  //          return reply.value;
  //        throw SomeException("No value");
  //      });
  //
  //  Future<Unit> fut3 = std::move(fut2)
  //      .thenValue([](string str) {
  //        cout << str << endl;
  //      })
  //      .thenTry([](folly::Try<string> strTry) {
  //        cout << strTry.value() << endl;
  //      })
  //      .thenError(folly::tag_t<std::exception>{}, [](std::exception const& e) {
  //        cerr << e.what() << endl;
  //      });
  folly::SemiFuture<GetReply> future_get(string key);

 private:
  folly::Promise<MemcacheClient::GetReply> getReplyPromise_;
};