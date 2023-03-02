#include "HelloFilter.h"

#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>

#define VDate "visitDate"

using namespace drogon;
class TimeFilter : public HttpFilter<TimeFilter> {
 public:
  virtual void doFilter(const HttpRequestPtr &req, FilterCallback &&cb, FilterChainCallback &&ccb) override {
    // 不合法，调用 cb
    // 合法时，调用 ccb

    auto &custonJson = app().getCustomConfig();
    if (custonJson.empty()) {
      LOG_ERROR << "empty custom config!";
    } else {
      LOG_TRACE << custonJson["realm"].asString();
    }

    trantor::Date now = trantor::Date::date();
    LOG_TRACE << "";
    if (req->session()->find(VDate)) {
      auto lastDate = req->session()->get<trantor::Date>(VDate);
      LOG_TRACE << "last:" << lastDate.toFormattedString(false);
      req->session()->modify<trantor::Date>(VDate, [now](trantor::Date &vdate) { vdate = now; });
      LOG_TRACE << "update visitDate";
      if (now > lastDate.after(10)) {
        // 10 sec later can visit again;
        ccb();
        return;
      } else {
        Json::Value json;
        json["result"] = "error";
        json["message"] = "Access interval should be at least 10 seconds";
        auto res = HttpResponse::newHttpJsonResponse(json);
        cb(res);
        LOG_ERROR << "Access interval should be at least 10 seconds";
        return;
      }
    }
    LOG_TRACE << "first access,insert visitDate";
    req->session()->insert(VDate, now);
    ccb();
  }
};
