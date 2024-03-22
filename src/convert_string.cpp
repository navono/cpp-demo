#include "convert_string.h"

#ifdef WIN32
#include <vector>
#include <locale>
#include <assert.h>
#include <codecvt>
#include <windows.h>
#include <stringapiset.h>

namespace utils{

namespace detail{

// 支持UCS2(char16_t or windows wchar_t)、UCS4(char32_t or linux wchar_t)
template<typename CharT>
std::basic_string<CharT> utf8_to_ucs(const std::string& source) {
  // 空字符串是合法的
  if (source.empty()) return std::basic_string<CharT>();

  // 定义转换器
  std::codecvt_utf8<CharT> converter;

  // 缓存
  std::mbstate_t the_mbstate = std::mbstate_t();
  const char* source_begin = source.c_str();
  const char* source_end = source_begin + source.size();
  const char* source_pos = NULL;
  int buffer_size = converter.length(
    the_mbstate,
    source_begin,
    source_end,
    source.size());

  // 转换
  std::vector<CharT> buffer(buffer_size);
  CharT* buffer_begin = buffer.data();
  CharT* buffer_end = buffer_begin + buffer.size();
  CharT* buffer_pos = nullptr;
  int ret_code = converter.in(the_mbstate, source_begin, source_end,
    source_pos, buffer_begin, buffer_end, buffer_pos);
  if (ret_code == std::codecvt_base::ok) {
    return std::basic_string<CharT>(buffer_begin, buffer_pos);
  }

  return std::basic_string<CharT>();
}

template<typename CharT>
std::string ucs_to_utf8(const std::basic_string<CharT>& source) {
  // 空字符串是合法的
  if (source.empty()) return std::string();

  // 定义转换器
  std::codecvt_utf8<CharT> converter;

  // 可能的最大长度与缓存
  size_t size_of_CharT = source.size() * converter.max_length();
  std::vector<char> buffer(size_of_CharT);

  // 转换
  const CharT* source_begin = source.c_str();
  const CharT* source_end = source_begin + source.size();
  const CharT* source_pos = nullptr;
  char* buffer_begin = buffer.data();
  char* buffer_end = buffer_begin + buffer.size();
  char* buffer_pos = nullptr;
  std::mbstate_t the_mbstate = std::mbstate_t();
  int ret_code = converter.out(the_mbstate, source_begin, source_end,
    source_pos, buffer_begin, buffer_end, buffer_pos);
  if (ret_code == std::codecvt_base::ok) {
    return std::string(buffer_begin, buffer_pos);
  }

  return std::string();
}

}  // namespace detail


std::wstring utf8_to_wstring(const std::string& source) {
  return detail::utf8_to_ucs<wchar_t>(source);
}

std::string wstring_to_utf8(const std::wstring& source) {
  return detail::ucs_to_utf8(source);
}


}  // namespace utils


#endif  // #ifdef WIN32
