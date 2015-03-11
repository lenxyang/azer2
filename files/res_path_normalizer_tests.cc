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
}

}  // namespace azer
