#include "MemcacheClient.h"

MemcacheClient::MemcacheClient() {}

MemcacheClient::GetReply MemcacheClient::get(string key) { return MemcacheClient::GetReply(); }

int MemcacheClient::async_get(string key, std::function<void(GetReply)> callback) { return 0; }

folly::SemiFuture<MemcacheClient::GetReply> MemcacheClient::future_get(string key) {
  return getReplyPromise_.getSemiFuture();
}
