#ifndef CONFIGGENERATIONSERVER_SRC_INCLUDE_COMMON_H_
#define CONFIGGENERATIONSERVER_SRC_INCLUDE_COMMON_H_

#include <filesystem>

#ifdef WIN32
#include <Windows.h>
#endif

// 获取当前模块的运行路径
inline std::filesystem::path GetModulePath() {
#if defined(_WIN32)
  // 对于 Windows 平台
  wchar_t buffer[MAX_PATH];
  GetModuleFileNameW(nullptr, buffer, MAX_PATH);
  return std::filesystem::path(buffer).parent_path();
#else
  // 对于其他平台（例如 Linux）
  //  char buffer[PATH_MAX];
  //  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  //  return std::filesystem::path(std::string(buffer, (count > 0) ? count : 0))
  //      .parent_path();
  return "";
#endif
}

inline std::string stringToUTF8(const std::string &str) {
  int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

  wchar_t *pwBuf = new wchar_t[nwLen + 1];
  ZeroMemory(pwBuf, nwLen * 2 + 2);

  ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

  int nLen =
      ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

  char *pBuf = new char[nLen + 1];
  ZeroMemory(pBuf, nLen + 1);

  ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

  std::string retStr(pBuf);

  delete[] pwBuf;
  delete[] pBuf;

  pwBuf = NULL;
  pBuf = NULL;

  return retStr;
}

inline std::string UTF8ToString(const std::string &str) {
  int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

  wchar_t *pwBuf = new wchar_t[nwLen + 1];
  memset(pwBuf, 0, nwLen * 2 + 2);

  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

  int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

  char *pBuf = new char[nLen + 1];
  memset(pBuf, 0, nLen + 1);

  WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

  std::string retStr = pBuf;

  delete[] pBuf;
  delete[] pwBuf;

  pBuf = NULL;
  pwBuf = NULL;

  return retStr;
}

inline std::string removeExtension(const std::string &filename) {
  size_t lastDotPos = filename.find_last_of('.');
  if (lastDotPos != std::string::npos) {
    return filename.substr(0, lastDotPos);
  } else {
    // 如果文件名中没有点（.），则没有后缀，返回原始文件名
    return filename;
  }
}

inline void stringReplace(std::string &str, const char *src, const char *dst) {
  size_t found = str.find(src);
  int src_size = strlen(src);
  int dst_size = strlen(dst);
  while (found != std::string::npos) {
    str.replace(found, src_size, dst);
    found = str.find(src, found + dst_size);
  }
}

#endif// CONFIGGENERATIONSERVER_SRC_INCLUDE_COMMON_H_
