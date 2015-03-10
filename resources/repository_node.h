#pragma once

#include <vector>
#include <map>

#include "base/memory/ref_counted.h"
#include "azer/resources/file_system.h"
#include "azer/base/resource.h"

namespace azer {

class RepositoryNode;
typedef scoped_refptr<RepositoryNode> RepositoryNodePtr;

// todo
// add StaticRepositoryNode which use dawg build the tree without any resources
class AZER_EXPORT RepositoryNode : public ::base::RefCounted<RepositoryNode> {
 public:
  RepositoryNode(const std::string& relative_path);
  ~RepositoryNode();

  void AddChild(RepositoryNodePtr child);
  void RemoveChild(RepositoryNodePtr child);
  bool HasAncestor(RepositoryNode* node) const;
  RepositoryNodePtr GetChild(const std::string& relative);

  const std::string& relative_path() const { return relative_path_;}

  RepositoryNode* root();
  const RepositoryNode* root() const;
  RepositoryNode* parent() { return parent_;}
  const RepositoryNode* parent() const { return parent_;}
  
  ResourcePtr GetResource(const StringType& path);
  RepositoryNodePtr GetResourceParent(const StringType& path);
  RepositoryNodePtr GetNodeFromPath(const StringType& path);
  RepositoryNodePtr GetNodeParent(const StringType& path);
 private:
  // get relative path's parent
  ResourcePtr GetLocalResource(const StringType& path);
  RepositoryNodePtr GetRelativeNode(const StringType& path);
  RepositoryNodePtr GetRelativeNodeParent(const StringType& path);
  RepositoryNodePtr GetNodeFromDirVec(const std::vector<StringType>& path);
  std::string relative_path_;
  RepositoryNode* parent_;
  RepositoryNodes children_;
  std::map<std::string, RepositoryNodePtr> children_;
  std::map<std::string, ResourcePtr> resource_dict_;
  DISALLOW_COPY_AND_ASSIGN(RepositoryNode);
};

}  // namespace azer
