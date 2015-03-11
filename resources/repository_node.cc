#include "azer/resources/repository_node.h"

#include <sstream>
#include "base/logging.h"

#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"

namespace azer {
namespace resources {

RepositoryNode::RepositoryNode(const StringType& name) 
    : name_(name)
    , parent_(NULL) {
}

RepositoryNode::RepositoryNode()
    : name_(FILE_PATH_LITERAL("/"))
    , parent_(NULL) {
}

RepositoryNode::~RepositoryNode() {
}

const RepositoryNode* RepositoryNode::root() const {
  auto cur = this;
  while (cur) {
    if (cur->parent_) {
      cur = cur->parent_;
    } else {
      return cur;
    }
  }

  return cur;
}

void RepositoryNode::AddChild(RepositoryNodePtr child) {
  DCHECK(child->parent_ == NULL);
  DCHECK(children_.end() == children_.find(child->name()));
  child->parent_ = this;
  children_.insert(std::make_pair(child->name(), child));
}

void RepositoryNode::RemoveChild(RepositoryNodePtr child) {
  DCHECK(child->parent_ == this);
  auto iter = children_.find(child->name());
  DCHECK(iter != children_.end());
  children_.erase(iter);
  child->parent_ = NULL;
}

RepositoryNodePtr RepositoryNode::GetChild(const StringType& name) {
  auto iter = children_.find(name);
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
  ::base::SplitString(path, FILE_PATH_LITERAL(':'), &vec);
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
  ::base::SplitString(path, FILE_PATH_LITERAL(':'), &vec);
  if (vec.size() == 2u) { 
    return GetRelativeNode(vec[0]);
  } else {
    return RepositoryNodePtr();
  }
}

RepositoryNodePtr RepositoryNode::GetNodeFromPath(const StringType& path) {
  if (StartsWith(path, FilePath::kRootPath, true)) {
    return root()->GetRelativeNode(path.substr(2));
  } else {
    return GetRelativeNode(path);
  }
}

RepositoryNodePtr RepositoryNode::GetRelativeNode(const StringType& path) {
  std::vector<StringType> vec;
  ::base::SplitString(path, FILE_PATH_LITERAL('/'), &vec);
  return RepositoryNode::GetNodeFromDirVec(vec);
}

RepositoryNodePtr RepositoryNode::GetNodeParent(const StringType& path) {
  if (StartsWith(path, FilePath::kRootPath, true)) {
    return GetRelativeNodeParent(path.substr(2));
  } else {
    return GetRelativeNodeParent(path);
  }
}

RepositoryNodePtr RepositoryNode::GetRelativeNodeParent(const StringType& path) {
  std::vector<StringType> vec;
  ::base::SplitString(path, '/', &vec);
  vec.erase(vec.begin() + vec.size() - 1);
  return GetNodeFromDirVec(vec);
}

RepositoryNodePtr RepositoryNode::GetNodeFromDirVec(
    const std::vector<StringType>& path) {
  RepositoryNodePtr ptr(this);
  for (auto iter = path.begin(); iter != path.end(); ++iter) {
    ptr = ptr->GetChild(*iter);
    if (!ptr.get()) {
      return ptr;
    }
  }

  return ptr;
}

StringType RepositoryNode::fullpath() const {
  StringType path;
  if (parent()) {
    path.append(parent()->fullpath());
    path.append(FILE_PATH_LITERAL("/"));
  }
  
  path.append(name());
  return path;
}

std::string RepositoryNode::PrintHierarchy(int ident) {
  std::stringstream ss;
  std::string ident_str(' ', ident * 2);
  if (parent()) {
    ss << ident_str << fullpath() << std::endl;
  }

  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    ss << iter->second->PrintHierarchy(ident);
  }
  return ss.str();
}
}  // namespace resources
}  // namespace azer
