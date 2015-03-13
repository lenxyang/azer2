#include "base/basictypes.h"
#include "azer/files/res_path_tokenizer.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(RESL) 
#undef RESL
#endif

#define RESL AZER_LITERAL 

namespace azer {
TEST(ResPathSplitter, Base) {
  StringType cases[] = {
    RESL(".://:."),
    RESL("c////cc"),
    RESL("c/b.:ef..."),
    RESL("//c////cc"),
  };

  const int kMaxTokens = 100;
  StringType expect_tokens[][kMaxTokens] = {
    {RESL("."), RESL(":"), RESL("//"), RESL(":"), RESL("."), RESL("\0")},
    {RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
    {RESL("c"), RESL("/"), RESL("b"), RESL("."), RESL(":"), RESL("ef"), 
     RESL("..."), RESL("\0")},
    {RESL("//"), RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathSplitter tokenizer(cases[i]);
    StringType* expect_token = expect_tokens[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(tokenizer.GetNext(), ResPathSplitter::kSuccess);
      ASSERT_EQ(tokenizer.token(), *expect_token);
      expect_token++;
    }
    ASSERT_EQ(tokenizer.GetNext(), ResPathSplitter::kNoTokens);
  }
}

TEST(ResPathSplitter, InvalidChar) {
  StringType cases[] = {
    RESL("-"),
  };

  const int kMaxTokens = 100;
  int expect_states[][kMaxTokens] = {
    {ResPathSplitter::kContainInvalidChar, ResPathSplitter::kNoTokens,},
  };

  StringType expect_tokens[][kMaxTokens] = {
    {RESL("-"),  RESL("\0"), }
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathSplitter tokenizer(cases[i]);
    int* expect_state = expect_states[i];
    StringType* expect_token = expect_tokens[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(tokenizer.GetNext(), *expect_state);
      ASSERT_EQ(tokenizer.token(), *expect_token);
      expect_token++;
      expect_state++;
    }
    ASSERT_EQ(tokenizer.GetNext(), *expect_state);
  }
}
}  // namespace azer
