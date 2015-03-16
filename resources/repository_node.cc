#include "azer/resources/repository_node.h"

#include <sstream>
#include "base/logging.h"

#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"

namespace azer {
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

RepositoryNodePtr RepositoryNode::FindOrCreate(const StringType& name) {
  auto iter = children_.find(name);
  if (iter != children_.end()) {
    return iter->second;
  } else {
    RepositoryNodePtr ptr(new RepositoryNode(name));
    children_.insert(std::make_pair(name, ptr));
    return ptr;
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


bool RepositoryNode::AddLocalResource(const StringType& name, ResourcePtr resource) {
  DCHECK(!name.empty());
  auto iter = resource_dict_.find(name);
  if (iter != resource_dict_.end()) {
    return false;
  }

  resource_dict_.insert(std::make_pair(name, resource));
  return true;
}

ResourcePtr RepositoryNode::GetLocalResource(const StringType& path) {
  auto iter = resource_dict_.find(path);
  if (iter != resource_dict_.end()) {
    return iter->second;
  } else {
    return ResourcePtr();
  }
}

ResourcePtr RepositoryNode::GetResource(const ResPath& path) {
  DCHECK_NE(path.type(), ResPath::kInvalidPath);
  RepositoryNodePtr node = GetNode(ResPath(path.filepath()));
  if (node.get()) {
    return node->GetLocalResource(path.component());
  }

  return ResourcePtr();
}

RepositoryNodePtr RepositoryNode::GetResourceParent(const ResPath& path) {
  DCHECK_NE(path.type(), ResPath::kInvalidPath);
  return GetNode(path.parent());
}

RepositoryNodePtr RepositoryNode::GetLocalNode(const StringType& name) {
  return GetChild(name);
}

RepositoryNodePtr RepositoryNode::GetNode(const ResPath& path) {
  DCHECK_NE(path.type(), ResPath::kInvalidPath);
  if (path.type() ==  ResPath::kAbsolutePath) {
    std::vector<StringType> vec;
    ::base::SplitString(path.filepath().substr(2), FILE_PATH_LITERAL('/'), &vec);
    return root()->RepositoryNode::GetNodeFromDirVec(vec);
  } else if (path.type() == ResPath::kRelativePath) {
    return GetLocalNode(path.filepath());
  } else {
    CHECK(false) << "not support";
    return RepositoryNodePtr();
  }
}

RepositoryNodePtr RepositoryNode::GetRelativeNode(const StringType& path) {
  std::vector<StringType> vec;
  ::base::SplitString(path, FILE_PATH_LITERAL('/'), &vec);
  return RepositoryNode::GetNodeFromDirVec(vec);
}

RepositoryNodePtr RepositoryNode::GetNodeParent(const ResPath& path) {
  DCHECK_NE(path.type(), ResPath::kInvalidPath);
  return GetNode(path.parent());
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

void GenerateTreeHierarchy(const ResPath& path, RepositoryNodePtr root) {
  DCHECK_EQ(path.type(), ResPath::kAbsolutePath);
  std::vector<StringType> vec;
  ::base::SplitString(path.filepath().substr(2), FILE_PATH_LITERAL('/'), &vec);
  RepositoryNodePtr ptr = root;
  for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
    if (iter->empty()) continue;
    ptr = ptr->FindOrCreate(*iter);
    DCHECK(ptr.get());
  }
}
}  // namespace azer
