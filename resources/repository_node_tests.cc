#include <iostream>
#include "testing/gtest/include/gtest/gtest.h"
#include "azer/files/test/test_util.h"
#include "azer/resources/repository_node.h"
#include "azer/resources/test/test_util.h"

namespace azer {
namespace resources {

using ::azer::test::Utf8ResPath;

TEST(RepositoryNode, AddChildAndRemove) {
  RepositoryNodePtr root(new RepositoryNode(FILE_PATH_LITERAL("/")));
  RepositoryNodePtr group1(new RepositoryNode(FILE_PATH_LITERAL("group1")));
  RepositoryNodePtr group2(new RepositoryNode(FILE_PATH_LITERAL("group2")));
  RepositoryNodePtr leaf1(new RepositoryNode(FILE_PATH_LITERAL("leaf1")));
  RepositoryNodePtr leaf2(new RepositoryNode(FILE_PATH_LITERAL("leaf2")));
  root->AddChild(group1);
  group1->AddChild(group2);
  group1->AddChild(leaf1);
  group2->AddChild(leaf2);
  std::cout << root->PrintHierarchy() << std::endl;
}

TEST(RepositoryNode, LoadRepositoryTreeFromString) {
  const char* hierarchy_tree = ""
      "//group1"
      "//group1/group2"
      "//group1/group2/leaf2"
      "//group1/leaf1"
      ;
  RepositoryNodePtr root = test::GenerateRepositoryTreeFromString(hierarchy_tree);
  std::cout << root->PrintHierarchy() << std::endl;
}

TEST(RepositoryNode, Parent) {
  const char* hierarchy_tree = ""
      "//group1"
      "//group1/group2"
      "//group1/group2/leaf2"
      "//group1/leaf1"
      "//group4"
      "//group4/group5"
      "//group4/group5/group6"
      "//group4/group5/group6/leaf3"
      ;
  RepositoryNodePtr root = test::GenerateRepositoryTreeFromString(hierarchy_tree);
  RepositoryNodePtr group4  = root->GetNode(Utf8ResPath("//group4"));
  RepositoryNodePtr leaf3  = root->GetNode(Utf8ResPath("//group4/group5/group6/leaf3"));
  ASSERT_TRUE(group4->HasAncestor(leaf3.get()));
  RepositoryNodePtr group6  = root->GetNode(Utf8ResPath("//group4/group5/group6"));
  RepositoryNodePtr group6_2  = root->GetNodeParent(
      Utf8ResPath("//group4/group5/group6/leaf3"));
  ASSERT_TRUE(leaf3->parent() == group6_2.get());
  ASSERT_TRUE(leaf3->parent() == group6.get());
  RepositoryNodePtr leaf3_2 = group6->GetNode(Utf8ResPath("leaf3"));
  ASSERT_TRUE(leaf3.get() == leaf3_2.get());
}

TEST(RepositoryNode, RemoveChild) {
  const char* hierarchy_tree = ""
      "//group1"
      "//group1/group2"
      "//group1/group2/leaf2"
      "//group1/leaf1"
      "//group4"
      "//group4/group5"
      "//group4/group5/group6"
      "//group4/group5/group6/leaf3"
      ;
  RepositoryNodePtr root = test::GenerateRepositoryTreeFromString(hierarchy_tree);
  RepositoryNodePtr group4 = root->GetNode(Utf8ResPath("//group4"));
  RepositoryNodePtr leaf3 = root->GetNode(Utf8ResPath("//group4/group5/group6/leaf3"));
  ASSERT_TRUE(group4->HasAncestor(leaf3.get()));
  RepositoryNodePtr group6 = root->GetNode(Utf8ResPath("//group4/group5/group6"));
  group6->RemoveChild(leaf3);
  ASSERT_FALSE(group4->HasAncestor(leaf3.get()));
  leaf3  = root->GetNode(Utf8ResPath("//group4/group5/group6/leaf3"));
  ASSERT_TRUE(leaf3.get() == NULL);
}

TEST(RepositoryNode, GenerateTreeHierarchy) {
  ResPath path("//group4/group5/group6/leaf3");
  RepositoryNodePtr root(new RepositoryNodePtr("//"));
  GenerateTreeHierarchy(path, root);
  RepositoryNodePtr leaf3 = root->GetNode(path.fullpath());
  ASSERT_TRUE(leaf3.get());
  ASSERT_TRUE(root->HasAncestor(leaf3.get());
}

}  // namespace resources {
}  // namespace azer
