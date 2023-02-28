#include "HelloFilter.h"

#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>

#define VDate "visitDate"

void HelloFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&ccb) {
  // 不合法，调用 fcb
  //  auto resp = HttpResponse::newHttpResponse();
  //  fcb(resp);

  // 合法时，调用 fccb
  ccb();
}
