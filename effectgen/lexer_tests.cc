#include "azer/effectgen/lexer.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace azer {
namespace effectgen {
TEST(Lexer, Base) {
  struct TestCase {
    const char* string;
    TokenType type;
    const char* data;
    int64_t ival;
  };

  TestCase test_case[] = {
    {"123456", TokenType::kInteger, "123456", 123456},
  };

  for (size_t i = 0; i < arraysize(test_case); ++i) {
    Lexer lexer(test_case[i].string);
    Token token;
    ASSERT_TRUE(lexer.GetNextToken(&token));
    ASSERT_EQ(token.type, test_case[i].type);
    ASSERT_EQ(token.data, test_case[i].data);
    ASSERT_EQ(token.ival, test_case[i].ival);
  }
}
}  // namespace effectgen
}  // namespace azer
