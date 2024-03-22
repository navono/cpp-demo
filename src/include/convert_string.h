
#ifndef UTILS_CONVERT_STRING_H_
#define UTILS_CONVERT_STRING_H_
#include <string>

#if !(defined(WIN32))
#define convert_string(str) str
#else
#define convert_string(str) utils::utf8_to_wstring(str)

namespace utils
{
/// @brief UTF8与wstring(UCS2 for Windows、UCS4 for Linux)转换
/// @param[in] source UTF8字符串
/// @return wstring字符串
std::wstring utf8_to_wstring(const std::string& source);

/// @brief UCS2与UTF8转换
/// @param[in] source UCS2字符串
/// @return UTF8字符串
std::string wstring_to_utf8(const std::wstring& source);

}  // namespace utils

#endif //WIN32

#endif  //UTILS_CONVERT_STRING_H_
