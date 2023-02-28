#include <drogon/drogon.h>

using namespace drogon;
class HelloFilter : public HttpFilter<HelloFilter> {
 public:
  virtual void doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) override {
    auto resp = HttpResponse::newHttpResponse();
    fcb(resp);
  }
};