#include "azer/resources/repository_node.h"

#include "base/logging.h"
#include "base/strings/string_util.h"
#include "base/strings/string_split.h"

namespace azer {
RepositoryNode::RepositoryNode(const std::string& relative_path) 
    : relative_path_(relative_path)
    , parent_(NULL) {
}

RepositoryNode::~RepositoryNode() {
}

RepositoryNode* root() {
  auto cur = this;
  while (cur) {
    if (cur->parent_) {
      cur = cur->parent_;
    }
  }

  return cur;
}

const RepositoryNode* root() const {
  auto cur = this;
  while (cur) {
    if (cur->parent_) {
      cur = cur->parent_;
    }
  }

  return cur;
}

void RepositoryNode::AddChild(RepositoryNodePtr child) {
  DCHECK(child->parent_ == NULL);
  DCHECK(children_.end() == children_.find(child->relative_path));
  child->parent_ = this;
  children_.insert(std::make_pair(child->relative_path, child));
}

void RepositoryNode::RemoveChild(RepositoryNodePtr child) {
  DCHECK(child->parent_ == this);
  auto iter = children_.find(child->relative_path());
  DCHECK(iter != children_.end());
  children_.erase(iter);
  child->parent_ = NULL;
}

RepositoryNodePtr RepositoryNode::GetChild(const std::string& relative) {
  auto iter = children_.find(child->relative_path());
  if (iter != children_.end()) {
    return iter->second;
  } else {
    return RepositoryNodePtr();
  }
}

bool RepositoryNode::HasAncestor(RepositoryNode* node) const {
  auto cur = node->parent();
  while (cur) {
    if (cur == this) {
      return true;
    }

    cur = cur->parent();
  }

  return false;
}

ResourcePtr RepositoryNode::GetLocalResource(const StringType& path) {
  auto iter = resource_dict_.find(path);
  if (iter != resource_dict_.end()) {
    return iter->second;
  } else {
    return ResourcePtr();
  }
}

ResourcePtr RepositoryNode::GetResource(const StringType& path) {
  std::vector<StringType> vec;
  SplitString(path, ':', &vec);
  if (vec.size() == 2u) { 
    RepositoryNodePtr node = GetNodeFromPath(vec[0]);
    if (node.get()) {
      return node->GetLocalResource(vec[1]);
    }
  }

  return ResourcePtr();
}

RepositoryNodePtr RepositoryNode::GetResourceParent(const StringType& path) {
  std::vector<StringType> vec;
  SplitString(path, ':', &vec);
  if (vec.size() == 2u) { 
    return GetNodeFromPath(vec[0]);
  } else {
    return ResourcePtr();
  }
}

RepositoryNodePtr RepositoryNode::GetNodeFromPath(const StringType& path) {
  if (::base::StartsWith(path, ResFilePath::kRootPath, true)) {
    return root->GetRelativeNode(path.substr(2));
  } else {
    return GetRelativeNode(path);
  }
}

RepositoryNodePtr RepositoryNode::GetRelativeNode(const StringType& path) {
  std::vector<StringType> vec;
  ::base::SplitString(path, '/', &vec);
  return RepositoryNode::GetNodeFromDirVec(vec);
}

RepositoryNodePtr RepositoryNode::GetNodeParent(const StringType& path) {
  if (::base::StartsWith(path, ResFilePath::kRootPath, true)) {
    return GetRelativePathParent(path.substr(2));
  } else {
    return GetRelativePathParent(path);
  }
}

RepositoryNodePtr GetRelativeNodeParent(const StringType& path) {
  std::vector<StringType> vec;
  ::base::SplitString(path, '/', &vec);
  vec.erase(vec.begin() + vec.length() - 1);
  return RepositoryNode::GetNodeFromDirVec(vec);
}

RepositoryNodePtr RepositoryNode::GetNodesFromDirVec(
    const std::vector<StringType>& path) {
  RepositoryNodePtr ptr(this);
  for (auto iter = path.begin(); iter != path.end(); ++iter) {
    ptr = ptr->GetChild(*iter);
    if (!ptr.get()) {
      return ptr;
    }
  }

  return true;
}

}  // namespace azer
