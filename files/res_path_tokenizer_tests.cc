#include "base/basictypes.h"
#include "azer/files/res_path_tokenizer.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(RESL) 
#undef RESL
#endif

#define RESL AZER_LITERAL 

namespace azer {
TEST(ResPathTokenizer, Base) {
  StringType cases[] = {
    RESL(".://:."),
    RESL("c////cc"),
    RESL("c/b.:ef..."),
    RESL("//c////cc"),
  };

  const int kMaxTokens = 100;
  int expect_tokens_num[] = {3, 4};
  StringType expect_tokens[][kMaxTokens] = {
    {RESL("."), RESL(":"), RESL("//"), RESL(":"), RESL(".")},
    {RESL("c"), RESL("////"), RESL("cc")},
    {RESL("c"), RESL("/"), RESL("b"), RESL("."), RESL(":"), RESL("ef"), RESL("...")},
    {RESL("//"), RESL("c"), RESL("////"), RESL("cc")},
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathTokenizer tokenizer(cases[i]);
    int expect_token_num = expect_tokens_num[i];
    for (size_t token_index = 0; token_index < expect_token_num; ++token_index) {
      ASSERT_EQ(tokenizer.GetNext(), ResPathTokenizer::kSuccess);
      ASSERT_EQ(tokenizer.token(), expect_tokens[i][token_index]);
    }
    ASSERT_EQ(tokenizer.GetNext(), ResPathTokenizer::kNoTokens);
  }
}
}  // namespace azer
