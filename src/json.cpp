#include "json.h"

#include <folly/dynamic.h>
#include <folly/json.h>

#include <nlohmann/json.hpp>

#include "dto.h"

void testDto(quill::Logger* logger) {
  try {
    {
      auto msg = json_dto::from_json<message_t>(json_data);

      auto tm = msg.m_when;
      const auto t = std::mktime(&tm);

      LOG_INFO(logger, "Deserialized from JSON: \t\nfrom:{} \t\nwhen: {}priority: {} \t\nimportance:{} \t\ntext: {}",
               msg.m_from, std::ctime(&t), msg.m_priority.get(), msg.m_importance, msg.m_text);
    }

    {
      auto t = std::time(nullptr);
      const message_t msg{"json_dto", *std::localtime(&t), "Hello once again!", 3, importance_levels::low};

      std::cout << "\nSerialized to JSON:\n" << json_dto::to_json(msg) << std::endl;
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "." << std::endl;
    return;
  }
}

namespace nh = nlohmann;

// Provide from_json and to_json functions in the same namespace as type
namespace ns {
struct book {
  std::string author;
  std::string title;
  double price;
  //  std::variant<bool, int> v;
};

void from_json(const nh::json& j, ns::book& val) {
  j.at("author").get_to(val.author);
  j.at("title").get_to(val.title);
  j.at("price").get_to(val.price);
  //  j.at("v").get_to(val.v);
}

void to_json(nh::json& j, const ns::book& val) {
  j["author"] = val.author;
  j["title"] = val.title;
}

}  // namespace ns

void testNlohmannJson(quill::Logger* logger) {
  const std::string s = R"(
[
    {
        "author" : "Haruki Murakami",
        "title" : "Kafka on the Shore",
        "price" : 25.17
    },
    {
        "author" : "Charles Bukowski",
        "title" : "Pulp",
        "price" : 22.48
    }
]
)";

  nh::json j = nh::json::parse(s);
  std::vector<ns::book> book_list = j.get<std::vector<ns::book>>();
  std::cout << "\n(1)\n";
  for (const auto& item : book_list) {
    std::cout << item.author << ", " << item.title << ", " << item.price << "\n";
  }
}

void testFollyDynamic(quill::Logger* logger) {
  // 先定义一个json字符串
  std::string jsonDocument = R"({"key":12,"key2":[false, null, true, "yay"]})";

  // 执行json反序列化，反序列化结果为dynamic
  folly::dynamic parsed = folly::parseJson(jsonDocument);

  for (const auto& i : parsed.items()) {
    LOG_INFO(logger, "key: {}", i.first.c_str());
  }

  //  assert(parsed["key"] == 12);
  //  assert(parsed["key2"][0] == false);
  //  assert(parsed["key2"][1] == nullptr);
  //  LOG_INFO(logger, "key: {}", parsed["key"].getInt());

  // 构建一个OBJECT
  folly::dynamic sonOfAJ =
      folly::dynamic::object("key", 12)("key2", folly::dynamic::array(false, nullptr, true, "yay"));

  // json序列化
  auto str = folly::toJson(sonOfAJ);
  LOG_INFO(logger, "str {}", str);

  auto dynamicArray = folly::dynamic::array();
  folly::dynamic obj1 = folly::dynamic::object;
  obj1["name"] = "M_1";
  obj1["value"] = "test";
  dynamicArray.push_back(obj1);

  folly::dynamic obj2 = folly::dynamic::object;
  obj2["name"] = "M_1";
  obj2["value"] = 12;
  dynamicArray.push_back(obj2);

  auto str2 = folly::toJson(dynamicArray);
  LOG_INFO(logger, "str2 {}", str2);
}