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

TEST(ResPathSplitter, TokenType) {
  StringType cases[] = {
    RESL(".://:."),
    RESL("c////cc"),
    RESL(".:ef..."),
  };

  const int kMaxTokens = 100;
  StringType expect_tokens[][kMaxTokens] = {
    {RESL("."), RESL(":"), RESL("//"), RESL(":"), RESL("."), RESL("\0")},
    {RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
    {RESL("."), RESL(":"), RESL("ef"), RESL("..."), RESL("\0")},
  };

  typedef ResPathSplitter Splitter;
  const int expect_types[][kMaxTokens] = {
    {Splitter::kDotToken, Splitter::kCommaToken, Splitter::kSlashToken,
     Splitter::kCommaToken, Splitter::kDotToken},
    {Splitter::kStringToken, Splitter::kSlashToken, Splitter::kStringToken},
    {Splitter::kDotToken, Splitter::kCommaToken, Splitter::kStringToken, 
     Splitter::kDotToken},
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
    RESL("//:.a"),
    RESL("c////cc"),
    RESL("//b:ef..."),
    RESL("//b:...ef..."),
    RESL("//b:.a.ef...b.c"),
    RESL("//c////cc"),
    RESL("content://hello"),
  };

  const int kMaxTokens = 100;
  StringType expect_tokens[][kMaxTokens] = {
    {RESL("//"), RESL(":.a"), RESL("\0")},
    {RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
    {RESL("//"), RESL("b"), RESL(":ef..."), RESL("\0")},
    {RESL("//"), RESL("b"), RESL(":...ef..."), RESL("\0")},
    {RESL("//"), RESL("b"), RESL(":.a.ef...b.c"), RESL("\0")},
    {RESL("//"), RESL("c"), RESL("////"), RESL("cc"), RESL("\0")},
    {RESL("content://"), RESL("hello"), RESL("\0")},
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathTokenizer tokenizer(cases[i]);
    StringType* expect_token = expect_tokens[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(tokenizer.GetNext(), ResPathTokenizer::kSuccess);
      ASSERT_EQ(tokenizer.token(), *expect_token);
      expect_token++;
    }
    ASSERT_EQ(tokenizer.GetNext(), ResPathTokenizer::kSuccess);
    ASSERT_EQ(tokenizer.type(), ResPathTokenizer::kEnd);
  }
}

TEST(ResPathTokenizer, Error) {
  StringType cases[] = {
    RESL("//:."),
  };

  const int kMaxTokens = 100;
  StringType expect_tokens[][kMaxTokens] = {
    {RESL("//"), RESL(":."), RESL("\0")},
  };

  int expect_types[][kMaxTokens] = {
    {ResPathTokenizer::kRoot, ResPathTokenizer::kErrorToken}, 
  };

  int expect_rets[][kMaxTokens] = {
    {ResPathTokenizer::kSuccess, ResPathTokenizer::kInvalidComponent},
  };
  
  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathTokenizer tokenizer(cases[i]);
    StringType* expect_token = expect_tokens[i];
    int* expect_type = expect_types[i];
    int* expect_ret = expect_rets[i];
    while (*expect_token != RESL("\0")) {
      ASSERT_EQ(tokenizer.GetNext(), *expect_ret);
      ASSERT_EQ(tokenizer.token(), *expect_token);
      ASSERT_EQ(tokenizer.type(), *expect_type);
      expect_token++;
    }
  }
}
}  // namespace files
}  // namespace azer
