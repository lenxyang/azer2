#include "base/basictypes.h"
#include "azer/files/res_path_tokenizer.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(RESL) 
#undef RESL
#endif

#define RESL AZER_LITERAL 

namespace azer {
namespace files {
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
    ResPathSplitter splitter(cases[i]);
    StringType* expect_token = expect_tokens[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(splitter.GetNext(), ResPathTokenizer::kSuccess);
      ASSERT_EQ(splitter.token(), *expect_token);
      expect_token++;
    }
    ASSERT_EQ(splitter.GetNext(), ResPathTokenizer::kNoTokens);
  }
}

TEST(ResPathSplitter, InvalidChar) {
  StringType cases[] = {
    RESL("-"),
  };

  const int kMaxTokens = 100;
  int expect_states[][kMaxTokens] = {
    {ResPathTokenizer::kContainInvalidChar, ResPathTokenizer::kNoTokens,},
  };

  StringType expect_tokens[][kMaxTokens] = {
    {RESL("-"),  RESL("\0"), }
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathSplitter splitter(cases[i]);
    int* expect_state = expect_states[i];
    StringType* expect_token = expect_tokens[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(splitter.GetNext(), *expect_state);
      ASSERT_EQ(splitter.token(), *expect_token);
      expect_token++;
      expect_state++;
    }
    ASSERT_EQ(splitter.GetNext(), *expect_state);
  }
}

TEST(ResPathTokenizer, Base) {
  StringType cases[] = {
    RESL("//:."),
    RESL("c////cc"),
    RESL("/b:ef..."),
    RESL("/b:...ef..."),
    RESL("/b:.a.ef...b.c"),
    RESL("//c////cc"),
  };

  const int kMaxTokens = 100;
  StringType expect_tokens[][kMaxTokens] = {
    {RESL("//"), RESL(":."), RESL("\0")},
    {RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
    {RESL("/"), RESL("b"), RESL(":"), RESL("ef..."), RESL("\0")},
    {RESL("/"), RESL("b"), RESL(":"), RESL("...ef..."), RESL("\0")},
    {RESL("/"), RESL("b"), RESL(":"), RESL(".a.ef...b.c"), RESL("\0")},
    {RESL("//"), RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathTokenizer splitter(cases[i]);
    StringType* expect_token = expect_tokens[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(splitter.GetNext(), ResPathTokenizer::kSuccess);
      ASSERT_EQ(splitter.token(), *expect_token);
      expect_token++;
    }
    ASSERT_EQ(splitter.GetNext(), ResPathTokenizer::kNoTokens);
  }
}
}  // namespace files
}  // namespace azer
