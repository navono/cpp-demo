#include "HelloFilter.h"

#include <drogon/drogon.h>

using namespace drogon;

namespace api::v1 {

// HttpControllers are automatically added to Drogon upon Drogon initializing.
class SayHello : public HttpController<SayHello> {
 public:
  METHOD_LIST_BEGIN
  // Drogon automatically appends the namespace and name of the controller to
  // the handlers of the controller. In this example, although we are adding
  // a handler to /. But because it is a part of the SayHello controller. It
  // ends up in path /SayHello/ (IMPORTANT! It is /SayHello/ not /SayHello
  // as they are different paths).
  METHOD_ADD(SayHello::genericHello, "/", Get, "HelloFilter");
  // Same for /hello. It ends up at /SayHello/hello
  //  METHOD_ADD(SayHello::personalizedHello, "/hello", Get);
  ADD_METHOD_TO(SayHello::personalizedHello, "/", Get);
  METHOD_LIST_END

 protected:
  void genericHello(const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(
        "Hello, this is a generic hello message from the SayHello "
        "controller");
    callback(resp);
  }

  void personalizedHello(const HttpRequestPtr &, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("Hi there, this is another hello from the SayHello Controller");
    callback(resp);
  }
};
}  // namespace api::v1