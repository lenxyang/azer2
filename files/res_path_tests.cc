#include "azer/files/res_path.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(RESL) 
#undef RESL
#endif

#define RESL AZER_LITERAL 

namespace azer {
TEST(ResPath, Base) {
  StringType cases[] = {
    RESL(""),
    RESL("root/group1"),
    RESL("//"),
    RESL("//root/group1"),
    RESL("//root/group1/group2:test"),
    RESL("content://root/group1/group2:test"),
  };

  const int kMaxTokens = 100;
  StringType expected[][kMaxTokens] = {
    {RESL(""), RESL(""), RESL(""), },
    {RESL("root/group1"), RESL("root/group1"), RESL(""), },
    {RESL("//"), RESL("//"), RESL(""), },
    {RESL("//root/group1"), RESL("//root/group1"), RESL(""),},
    {RESL("//root/group1/group2:test"), RESL("//root/group1/group2"), RESL(":test"),},
    {RESL("content://root/group1/group2:test"), RESL("root/group1/group2"), RESL(":test"),},
  };

  ResPath::PathType type[] = {
    ResPath::kRelativePath, 
    ResPath::kRelativePath, 
    ResPath::kAbsolutePath, 
    ResPath::kAbsolutePath, 
    ResPath::kAbsolutePath, 
    ResPath::kProtoPath, 
  };

  
  for (size_t i = 0; i < arraysize(cases); ++i) {
    StringType* expect = expected[i];
    ResPath path(cases[i]);
    ASSERT_EQ(path.fullpath(), expect[0]);
    ASSERT_EQ(path.filepath(), expect[1]);
    ASSERT_EQ(path.component(), expect[2]);
  }
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
