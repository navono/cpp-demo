#include "utils/XLogger.h"

#ifdef _WIN32
#include <Windows.h>
#include <afx.h>
#include <afxwin.h>
#endif

static const wchar_t m_szName[] = L"����";
// �����ڻص����� ���ֱ���� DefWindowProc, �رմ���ʱ���������Ϣѭ��
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);// �رմ���ʱ����WM_QUIT��Ϣ������Ϣѭ��
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
  RegisterClass(&wc);// �ǼǴ�����

  HWND hWnd;
  hWnd = CreateWindow(m_szName, m_szName,
                      WS_OVERLAPPEDWINDOW,
                      0,                                  // ���� X ����
                      0,                                  // ���� Y ����
                      0,                                  // ���ڿ��
                      0,                                  // ���ڸ߶�
                      nullptr, nullptr, nullptr, nullptr);// ��������
  ShowWindow(hWnd, SW_HIDE);                              // ���ش���

  MSG sMsg;
  while (int ret = GetMessage(&sMsg, nullptr, 0, 0)) {// ��Ϣѭ��
    if (ret != -1) {
      TranslateMessage(&sMsg);
      DispatchMessage(&sMsg);
    }
  }
  return 0;
}
