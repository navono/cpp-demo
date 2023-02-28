#include "HelloFilter.h"

void HelloFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
  // 不合法，调用 fcb
  //  auto resp = HttpResponse::newHttpResponse();
  //  fcb(resp);

  // 合法时，调用 fccb
  fccb();
}
