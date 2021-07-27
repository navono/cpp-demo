#include <iostream>
#include <regex>
#include <string>

using namespace std;

void simple1(const string& str) {
  smatch result;
  regex pattern(R"((\w+)\.\$(\d{4}))");  // 匹配5个任意单字符 + 下划线 + 4个数字

  if (regex_match(str, result, pattern)) {
    cout << result[0] << endl;  // 完整匹配结果，Hello_2018
    cout << result[1] << endl;  // 第一组匹配的数据，Hello
    cout << result[2] << endl;  // 第二组匹配的数据，2018
                                //    cout << "结果显示形式2" << endl;
                                //    cout << result.str() << endl;   // 完整结果，Hello_2018
                                //    cout << result.str(1) << endl;  // 第一组匹配的数据，Hello
                                //    cout << result.str(2) << endl;  // 第二组匹配的数据，2018
  }

  // 遍历结果
  //  for (const auto& i : result) {
  //    cout << i << endl;
  //  }
}

void simple2(const string& str) {
  smatch result;
  regex pattern(R"(([\w-]+)((\.\$)([\w-]+))?)");

  //  regex pattern(R"(.(\w-)(.$))");
  //  regex pattern(R"((.\w)(.$))");

  regex_match(str, result, pattern);
  // 遍历结果
  for (const auto& i : result) {
    cout << i << endl;
  }
}

void testRegex() {
  //  simple1("Hello.$2018");
  simple2("Ma-_1.$Unit");
  simple2("Ma-_2");
}

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest.h"
TEST_CASE("we can have tests written here, to test impl. details") { CHECK(true); }
#endif