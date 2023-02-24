#include <iostream>
#include <cppzmq/zmq.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

#else
int main(int argc, char **argv) {
  Config::initializeArgs(argc, argv);
  //  NUIKIT_DEBUG(fmt::format("optDaemon: {}, optDebug: {}", Config::optDaemon, Config::optDebug));

#endif

  zmq::context_t ctx;
  zmq::socket_t sock(ctx, zmq::socket_type::push);
  sock.bind("inproc://test");
  sock.send(zmq::str_buffer("Hello, world"), zmq::send_flags::dontwait);

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
