#include "utils/XLogger.h"

#ifdef _WIN32
#include <Windows.h>
#include <afx.h>
#include <afxwin.h>
#endif

static const wchar_t m_szName[] = L"窗口";
// 主窗口回调函数 如果直接用 DefWindowProc, 关闭窗口时不会结束消息循环
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);// 关闭窗口时发送WM_QUIT消息结束消息循环
      break;
    case WM_NCCREATE: {
      XLOG_TRACE("window initialized");
      break;
    }
    default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return 0;
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

  WNDCLASS wc;
  memset(&wc, 0, sizeof(WNDCLASS));
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.lpfnWndProc = (WNDPROC) WindowProc;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH) (COLOR_WINDOW);
  wc.lpszClassName = m_szName;
  RegisterClass(&wc);// 登记窗口类

  HWND hWnd;
  hWnd = CreateWindow(m_szName, m_szName,
                      WS_OVERLAPPEDWINDOW,
                      0,                                  // 窗口 X 坐标
                      0,                                  // 窗口 Y 坐标
                      0,                                  // 窗口宽度
                      0,                                  // 窗口高度
                      nullptr, nullptr, nullptr, nullptr);// 创建窗口
  ShowWindow(hWnd, SW_HIDE);                              // 隐藏窗口

  MSG sMsg;
  while (int ret = GetMessage(&sMsg, nullptr, 0, 0)) {// 消息循环
    if (ret != -1) {
      TranslateMessage(&sMsg);
      DispatchMessage(&sMsg);
    }
  }
  return 0;
}
