//#include <BS_thread_pool.hpp>
//#include <BS_thread_pool_utils.hpp>
#include <chrono>// std::chrono
//#include <fmt/core.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>
#include <hv/HttpServer.h>
#include <thread>// std::this_thread

#include "utils/XLogger.h"

//#ifdef _WIN32
//#include <Windows.h>
//#endif

//#ifdef _WIN32
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
//                      _In_ int nCmdShow) {
//  UNREFERENCED_PARAMETER(hPrevInstance);
//  UNREFERENCED_PARAMETER(lpCmdLine);
//
//#else
//int main(int argc, char **argv) {
//#endif

//BS::synced_stream sync_out;
//BS::thread_pool pool(4);
//
//void sleep_half_second(const int i) {
//  std::this_thread::sleep_for(std::chrono::milliseconds(500));
//  sync_out.println("Task ", i, " done.");
//}
//
//void monitor_tasks() {
//  sync_out.println(pool.get_tasks_total(), " tasks total, ", pool.get_tasks_running(), " tasks running, ", pool.get_tasks_queued(), " tasks queued.");
//}

using namespace hv;
int main(int argc, char **argv) {
  XLOG_TRACE("Hello");

  Poco::MD5Engine md5;
  Poco::DigestOutputStream ds(md5);
  ds << "abcdefghijklmnopqrstuvwxyz";
  ds.close();
  auto digest = Poco::DigestEngine::digestToHex(md5.digest());
  XLOG_TRACE("digest: {}", digest);

  HttpService router;
  router.GET("/ping", [](HttpRequest *req, HttpResponse *resp) {
    return resp->String("pong");
  });

  XLOG_TRACE("http server running at: 8080");
  HttpServer server(&router);
  server.setPort(8080);
  server.setThreadNum(4);
  server.run();

  //  pool.wait();
  //  pool.detach_sequence(0, 12, sleep_half_second);
  //  monitor_tasks();
  //  std::this_thread::sleep_for(std::chrono::milliseconds(750));
  //  monitor_tasks();
  //  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  //  monitor_tasks();
  //  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  //  monitor_tasks();

  return 0;
}
