#include <iostream>
#include "testing/gtest/include/gtest/gtest.h"
#include "azer/resources/repository_node.h"

namespace azer {

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

TEST(RepositoryNode, Path) {
}

}  // namespace azer
