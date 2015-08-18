#pragma once

#include <vector>
#include <map>

#include "base/memory/ref_counted.h"
#include "azer/files/file_system.h"
#include "azer/base/resource.h"

namespace azer {
class RepositoryNode;
typedef scoped_refptr<RepositoryNode> RepositoryNodePtr;

// todo
// add StaticRepositoryNode which use dawg build the tree without any resources
class AZER_EXPORT RepositoryNode : public ::base::RefCounted<RepositoryNode> {
 public:
  RepositoryNode();
  explicit RepositoryNode(const StringType& name);
  ~RepositoryNode();

  void AddChild(RepositoryNodePtr child);
  void RemoveChild(RepositoryNodePtr child);
  bool HasAncestor(RepositoryNode* node) const;
  RepositoryNodePtr GetChild(const StringType& relative);
  RepositoryNodePtr FindOrCreate(const StringType& name);
  RepositoryNodePtr FindOrCreateRecusive(const StringType& name);

  const StringType& name() const { return name_;}
  StringType fullpath() const;

  const RepositoryNode* root() const;
  RepositoryNode* root() {
    return const_cast<RepositoryNode*>(
        const_cast<const RepositoryNode*>(this)->root());
  }

  RepositoryNode* parent() { return parent_;}
  const RepositoryNode* parent() const { return parent_;}

  // get resource
  bool AddLocalResource(const StringType& name, ResourcePtr& resource);
  bool AddResource(const ResPath& path, ResourcePtr& resource);
  ResourcePtr GetLocalResource(const StringType& path);
  ResourcePtr GetResource(const ResPath& path);
  RepositoryNodePtr GetResourceParent(const ResPath& path);

  // get nodes
  RepositoryNodePtr GetLocalNode(const StringType& name);
  RepositoryNodePtr GetNode(const ResPath& path);
  RepositoryNodePtr GetNodeParent(const ResPath& path);
  RepositoryNodePtr GetRelativeNode(const StringType& path);

  std::string PrintHierarchy(int ident = 0);
 private:
  // get relative path's parent
  RepositoryNodePtr GetNodeFromDirVec(const std::vector<StringType>& path);
  StringType name_;
  RepositoryNode* parent_;
  std::map<StringType, RepositoryNodePtr> children_;
  std::map<StringType, ResourcePtr> resource_dict_;
  DISALLOW_COPY_AND_ASSIGN(RepositoryNode);
};

// generate tree hierarchy if the node in path not exits
void AZER_EXPORT GenerateTreeHierarchy(const ResPath& path, RepositoryNodePtr root);
}  // namespace azer
