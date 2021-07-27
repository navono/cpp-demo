#include "example.h"

#include "dto.h"

Dummy::Dummy() = default;

bool Dummy::doSomething() {
  // Do silly things, using some C++17 features to enforce C++17 builds only.
  constexpr int digits[2] = {0, 1};
  auto [zero, one] = digits;
  return zero + one;
}

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

#ifdef ENABLE_DOCTEST_IN_LIBRARY
#include "doctest.h"
TEST_CASE("we can have tests written here, to test impl. details") { CHECK(true); }
#endif
