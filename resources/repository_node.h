#pragma once

#include <vector>
#include <map>

#include "base/memory/ref_counted.h"
#include "azer/files/file_system.h"
#include "azer/base/resource.h"

namespace azer {
namespace resources {

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

  const StringType& name() const { return name_;}
  StringType fullpath() const;

  const RepositoryNode* root() const;
  RepositoryNode* root() {
    return const_cast<RepositoryNode*>(
        const_cast<const RepositoryNode*>(this)->root());
  }


  RepositoryNode* parent() { return parent_;}
  const RepositoryNode* parent() const { return parent_;}
  
  ResourcePtr GetResource(const ResPath& path);
  RepositoryNodePtr GetResourceParent(const ResPath& path);
  RepositoryNodePtr GetLocalNode(const StringType& name);
  RepositoryNodePtr GetNode(const ResPath& path);
  RepositoryNodePtr GetNodeParent(const ResPath& path);
  RepositoryNodePtr GetRelativeNode(const StringType& path);

  std::string PrintHierarchy(int ident = 0);
 private:
  // get relative path's parent
  ResourcePtr GetLocalResource(const StringType& path);
  RepositoryNodePtr GetNodeFromDirVec(const std::vector<StringType>& path);
  StringType name_;
  RepositoryNode* parent_;
  std::map<StringType, RepositoryNodePtr> children_;
  std::map<StringType, ResourcePtr> resource_dict_;
  DISALLOW_COPY_AND_ASSIGN(RepositoryNode);
};
}  // namespace resources
}  // namespace azer
