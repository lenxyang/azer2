#include "azer/files/res_path.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace azer {
TEST(ResPath, Base) {
  ResPath path1("//");
  ResPath path2("//root/group1");
  ResPath path2("//root/group1/group2:test");
}

TEST(ResPath, Normalize) {
  ResPath path("//");
  path.Append("a/");
  path.Append("./");
  path.Append("..");
}
}  // namespace azer
