#include "utils/XLogger.h"
#include <fmt/core.h>

#ifdef _WIN32
#include <Windows.h>
#endif

DWORD g_dwMainThreadId;
BOOL WINAPI ConsoleCtrlHandler(
    DWORD dwCtrlType) {
  if (dwCtrlType == CTRL_CLOSE_EVENT) {
    //PostQuitMessage(0);
    PostThreadMessage(g_dwMainThreadId, WM_QUIT, 0, 0);
    return TRUE;
  }
  return FALSE;
}

#ifdef _WIN32
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

#else
int main(int argc, char **argv) {
#endif

  XLOG_TRACE("Hello");
  fmt::print("Hello");

  g_dwMainThreadId = GetCurrentThreadId();
  SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);
  //TODO: Any initialize code here:
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  SetConsoleCtrlHandler(ConsoleCtrlHandler, FALSE);
  //TODO: Any code to release resources here:
  return 0;
}
