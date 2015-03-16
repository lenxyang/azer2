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

  ResPathNormalizer normalizer;
  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPath path(cases[i]);
    ASSERT_TRUE(normalizer.Normalize(&path));
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

  ResPathNormalizer normalizer;
  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPath path(cases[i]);
    ASSERT_TRUE(normalizer.Normalize(&path));
  }
}

TEST(ResPathNormalizer, DirDot) {
  StringType cases[] = {
    AZER_LITERAL("//a/./c:efg"),
    AZER_LITERAL("//a/..//c:efg"),
    AZER_LITERAL("//a/../c:efg"),
  };

  ResPathNormalizer normalizer;
  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPath path(cases[i]);
    ASSERT_TRUE(normalizer.Normalize(&path));
  }
}

TEST(ResPathNormalizer, ProtoPath) {
  ResPathNormalizer normalizer;
  {
    ResPath path(AZER_LITERAL("rpc://www:d"));
    ASSERT_TRUE(normalizer.Normalize(&path));
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

  ResPathNormalizer normalizer;
  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPath path(cases[i]);
    ASSERT_TRUE(normalizer.Normalize(&path));
  }
}
}  // namespace azer
