#include "base/basictypes.h"
#include "azer/files/res_path_normalizer.h"
#include "azer/files/res_path.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace azer {
TEST(ResPathNormalizer, Base) {
  StringType cases[] = {
    AZER_LITERAL("c"),
    AZER_LITERAL(":c"),
    AZER_LITERAL("//"),
    AZER_LITERAL("//a/b/c"),
    AZER_LITERAL("//a/b/c:efg"),
    AZER_LITERAL("rpc://ccc"),
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathNormalizer normalizer(cases[i]);
    normalizer.Normalize();
    ASSERT_TRUE(normalizer.success());
  }
}

TEST(ResPathNormalizer, FileNameWithDot) {
  StringType cases[] = {
    AZER_LITERAL("B/.c/d"),
    AZER_LITERAL("B/..c/d"),
    AZER_LITERAL("B/...c/d"),
    AZER_LITERAL("B/....c/d"),
    AZER_LITERAL("B/.c.exe/d"),
    AZER_LITERAL("B/a.b.c.d.e.f.exe/d"),
    AZER_LITERAL("//B/.c/d"),
    AZER_LITERAL("//B/..c/d"),
    AZER_LITERAL("//B/...c/d"),
    AZER_LITERAL("//B/....c/d"),
    AZER_LITERAL("//B/.c.exe/d"),
    AZER_LITERAL("//B/a.b.c.d.e.f.exe/d"),
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathNormalizer normalizer(cases[i]);
    normalizer.Normalize();
    ASSERT_TRUE(normalizer.success());
  }
}

TEST(ResPathNormalizer, DirDot) {
  StringType cases[] = {
    AZER_LITERAL("//a/./c:efg"),
    AZER_LITERAL("//a/..//c:efg"),
    AZER_LITERAL("//a/../c:efg"),
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathNormalizer normalizer(cases[i]);
    normalizer.Normalize();
    ASSERT_TRUE(normalizer.success());
  }
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
    AZER_LITERAL("/a"),
    AZER_LITERAL("//a:b:c"),
    AZER_LITERAL("//a/b/:c"),
    AZER_LITERAL("//a:http://"),
    AZER_LITERAL("//B/.../d"),
    AZER_LITERAL("//B/c/d"),
    AZER_LITERAL("//B/c/d&"),
    AZER_LITERAL("//B/c/123d&"),
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPathNormalizer normalizer(cases[i]);
    normalizer.Normalize();
    ASSERT_FALSE(normalizer.success()) << cases[i];
  }
}
}  // namespace azer
