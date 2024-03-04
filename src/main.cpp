#include <chrono>// std::chrono
//#include <fmt/core.h>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>
#include <Poco/Notification.h>
#include <Poco/NotificationCenter.h.>
#include <Poco/NotificationQueue.h>
#include <Poco/ThreadPool.h>
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

using namespace hv;

// 定义一个自定义的通知类
class DataNotification : public Poco::Notification {
 public:
  explicit DataNotification(int data) : _data(data) {}
  int data() const { return _data; }

 private:
  int _data;
};

// 定义一个特殊的退出通知类
class StopNotification : public Poco::Notification {};

class MyTask : public Poco::Runnable {
 public:
  explicit MyTask(Poco::NotificationQueue &queue) : _queue(queue), _running(true) {}

  void run() override {
    XLOG_TRACE("MyTask run, waiting data from main thread...");
    while (_running) {
      Poco::Notification::Ptr pNotification(_queue.waitDequeueNotification());

      if (pNotification) {
        if (dynamic_cast<StopNotification *>(pNotification.get())) {
          // 收到退出通知，停止运行
          _running = false;
          continue;
        }

        auto *pDataNotification = dynamic_cast<DataNotification *>(pNotification.get());
        if (pDataNotification) {
          // 处理数据
          XLOG_TRACE("Received data from main thread: {}", pDataNotification->data());
        }
      }
    }

    XLOG_TRACE("Exit MyTask run...");
  }

 private:
  Poco::NotificationQueue &_queue;
  bool _running;
};

int main(int argc, char **argv) {
  XLOG_TRACE("Hello");

  Poco::MD5Engine md5;
  Poco::DigestOutputStream ds(md5);
  ds << "abcdefghijklmnopqrstuvwxyz";
  ds.close();
  auto digest = Poco::DigestEngine::digestToHex(md5.digest());
  XLOG_TRACE("digest: {}", digest);

  Poco::NotificationQueue queue;
  // 创建并启动子线程
  MyTask worker(queue);
  Poco::Thread thread;
  thread.start(worker);

  HttpService router;
  router.GET("/ping", [&queue](HttpRequest *req, HttpResponse *resp) {
    queue.enqueueNotification(new DataNotification(123));
    return resp->String("pong");
  });
  router.GET("/stop", [&queue](HttpRequest *req, HttpResponse *resp) {
    queue.enqueueNotification(new StopNotification());
    return resp->String("stopped");
  });

  XLOG_TRACE("http server running at: 8080");
  HttpServer server(&router);
  server.setPort(8080);
  server.setThreadNum(4);
  server.run();

  return 0;
}
