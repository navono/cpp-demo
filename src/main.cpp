#include "utils/XLogger.h"
#include <fmt/core.h>

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

int main(int argc, char **argv) {
  XLOG_TRACE("Hello");
  fmt::print("Hello");

  return 0;
}
