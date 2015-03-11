#include "base/basictypes.h"
#include "azer/files/res_path_normalizer.h"
#include "azer/files/res_path.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace azer {
TEST(ResPathNormalizer, Base) {
  {
    ResPathNormalizer normalizer(AZER_LITERAL("//a/b/c:d"));
    normalizer.Normalize();
    ASSERT_TRUE(normalizer.success());
    ASSERT_EQ(normalizer.component(), AZER_LITERAL("d"));
  }
}

TEST(ResPathNormalizer, Dot) {
}

TEST(ResPathNormalizer, ProtoPath) {
  {
    ResPathNormalizer normalizer(AZER_LITERAL("rpc://www:d"));
    normalizer.Normalize();
    ASSERT_TRUE(normalizer.success());
    ASSERT_EQ(normalizer.component(), AZER_LITERAL("d"));
  }
}

TEST(ResPathNormalizer, Failed) {
  StringType cases[] = {
    AZER_LITERAL("//a:b:c"),
    AZER_LITERAL("//a:http://"),
    AZER_LITERAL("/B/c/d"),
    AZER_LITERAL("/B/c/d&"),
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathNormalizer normalizer(cases[i]);
    normalizer.Normalize();
    ASSERT_FALSE(normalizer.success());
  }
}
}  // namespace azer
