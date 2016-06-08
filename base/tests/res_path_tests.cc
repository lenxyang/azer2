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

TEST(ResPath, Parent) {
  StringType cases[] = {
    RESL("root/group1"),
    RESL("//root"),
    RESL("//root/group1"),
    RESL("//root/group1/group2:test"),
    RESL("content://root/group1/group2:test"),
  };

  const int kMaxTokens = 100;
  StringType expect_path[][kMaxTokens] = {
    {RESL("root"), RESL("root"),},
    {RESL("//"), RESL("//"),},
    {RESL("//root"), RESL("//root"),},
    {RESL("//root/group1"), RESL("//root/group1"),},
    {RESL("content://root/group1"), RESL("root/group1"),},
  };

  ResPath::PathType expect_type[] = {
    ResPath::kRelativePath,
    ResPath::kAbsolutePath,
    ResPath::kAbsolutePath, 
    ResPath::kAbsolutePath, 
    ResPath::kProtoPath, 
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPath path(cases[i]);
    ResPath parent = path.parent();
    StringType* expect = expect_path[i];
    ASSERT_EQ(parent.fullpath(), expect[0]);
    ASSERT_EQ(parent.filepath(), expect[1]);
    ASSERT_EQ(parent.component(), RESL(""));
    ASSERT_EQ(parent.type(), expect_type[i]);
  }
}

TEST(ResPath, InvalidParent) {
  StringType cases[] = {
    RESL(""),
    RESL("//"),
    RESL("content://"),
  };

  const int kMaxTokens = 100;
  StringType expect_path[][kMaxTokens] = {
    {RESL(""), RESL(""),},
    {RESL(""), RESL(""),},
    {RESL(""), RESL(""),},
  };

  ResPath::PathType expect_type[] = {
    ResPath::kInvalidPath,
    ResPath::kInvalidPath,
    ResPath::kInvalidPath,
  };

  for (size_t i = 0; i < arraysize(cases); ++i) {
    ResPath path(cases[i]);
    ResPath parent = path.parent();
    StringType* expect = expect_path[i];
    ASSERT_EQ(parent.fullpath(), expect[0]);
    ASSERT_EQ(parent.filepath(), expect[1]);
    ASSERT_EQ(parent.component(), RESL(""));
    ASSERT_EQ(parent.type(), expect_type[i]);
  }
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
