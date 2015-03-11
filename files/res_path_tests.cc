#include "azer/files/res_path.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace azer {
TEST(ResPath, Base) {
  ResPath path1(AZER_LITERAL("//"));
  ResPath path2(AZER_LITERAL("//root/group1"));
  ResPath path3(AZER_LITERAL("//root/group1/group2:test"));
}

TEST(ResPath, InvalidPath) {
}

TEST(ResPath, PathType) {
}

TEST(ResPath, Normalize) {
  ResPath path(AZER_LITERAL("//"));
  path.Append(ResPath(AZER_LITERAL("a/")));
  path.Append(ResPath(AZER_LITERAL("./")));
  path.Append(ResPath(AZER_LITERAL("..")));
}
}  // namespace azer
