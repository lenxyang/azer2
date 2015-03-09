#pragma once

#include <vector>
#include <map>

#include "base/memory/ref_counted.h"
#include "azer/resources/file_system.h"
#include "azer/base/resource.h"

namespace azer {

class RepositoryNode;
typedef scoped_refptr<RepositoryNode> RepositoryNodePtr;

class AZER_EXPORT RepositoryNode : public ::base::RefCounted<RepositoryNode> {
 public:
  RepositoryNode();
  ~RepositoryNode();

  void AddChild(RepositoryNodePtr child);
  void RemoveChild(RepositoryNodePtr child);

  RepositoryNodePtr& GetNode(const std::string& path);
  RepositoryNodePtr& GetParent(const std::string& path);
 private:
  RepositoryNode* parent_;
  RepositoryNodes children_;
  std::map<std::string, RepositoryNodePtr> children_;
  std::map<std::string, ResourcePtr> resource_dict_;
  DISALLOW_COPY_AND_ASSIGN(RepositoryNode);
};
}  // namespace azer
