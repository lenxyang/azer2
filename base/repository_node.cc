#include "azer/base/repository_node.h"

#include <sstream>
#include "base/logging.h"

#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"

namespace azer {
RepositoryNode::RepositoryNode(const StringType& name)
    : name_(name)
    , parent_(NULL)
    , user_data_(NULL) {
}

RepositoryNode::RepositoryNode()
    : name_(FILE_PATH_LITERAL("/"))
    , parent_(NULL) {
}

RepositoryNode::~RepositoryNode() {
  children_.clear();
  // LOG(ERROR) << "free all: " << fullpath();
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

int32_t RepositoryNode::GetIndexOf(const RepositoryNodePtr& node) const {
  int index = 0;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter, ++index) {
    if (iter->second.get() == node.get())
      return index;
  }
  return -1;
}

RepositoryNodePtr RepositoryNode::child_at(int32_t index) {
  DCHECK(index < child_count());
  int cur = 0;
  for (auto iter = children_.begin(); iter != children_.end(); ++iter, ++cur) {
    if (cur == index)
      return iter->second;
  }

  NOTREACHED();
  return RepositoryNodePtr();
}

int32_t RepositoryNode::child_count() const {
  return static_cast<int32_t>(children_.size());
}

RepositoryNodePtr RepositoryNode::FindOrCreate(const StringType& name) {
  auto iter = children_.find(name);
  if (iter != children_.end()) {
    return iter->second;
  } else {
    RepositoryNodePtr ptr(new RepositoryNode(name));
    ptr->parent_ = this;
    children_.insert(std::make_pair(name, ptr));
    return ptr;
  }
}

RepositoryNodePtr RepositoryNode::FindOrCreateRecusive(const StringType& name) {
  ResPath path(name);
  std::vector<Slice> dirs = path.dirs();
  RepositoryNodePtr ptr = this;
  for (auto iter = dirs.begin(); iter != dirs.end(); ++iter) {
    if (iter->as_string() != FILE_PATH_LITERAL("//"))  {
      ptr = ptr->FindOrCreate(iter->as_string());
    } else {
      DCHECK(iter == dirs.begin());
      ptr = root();
    }
  }
  Slice filename = path.BaseName();
  if (!filename.empty()) {
    ptr->FindOrCreate(filename.as_string());
  }

  return ptr;
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


bool RepositoryNode::AddLocalResource(const StringType& name, VariantResource res) {
  auto iter = resource_dict_.find(name);
  if (iter != resource_dict_.end()) {
    return false;
  }

  resource_dict_.insert(std::make_pair(name, res));
  return true;
}

bool RepositoryNode::AddResource(const ResPath& path, VariantResource res) {
  RepositoryNodePtr node = GetNode(ResPath(path.filepath().as_string()));
  if (node.get()) {
    if (path.IsAbsolutePath()) {
      GenerateTreeHierarchy(path, root());
    } else {
      GenerateTreeHierarchy(path, this);
    }
    node = GetNode(ResPath(path.filepath().as_string()));
    DCHECK(node.get());
  }

  node->AddLocalResource(path.component().as_string(), res);
  return true;
}

VariantResource RepositoryNode::GetLocalResource(const StringType& path) {
  auto iter = resource_dict_.find(path);
  if (iter != resource_dict_.end()) {
    return iter->second;
  } else {
    return VariantResource();
  }
}

VariantResource RepositoryNode::GetResource(const ResPath& path) {
  DCHECK_NE(path.type(), ResPath::kInvalidPath);
  RepositoryNodePtr node = GetNode(ResPath(path.filepath().as_string()));
  if (node.get()) {
    return node->GetLocalResource(path.component().as_string());
  } else {
    return VariantResource();
  }
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
    std::vector<StringType> vec = ::base::SplitString(
        path.filepath().substr(2).as_string(), FILE_PATH_LITERAL("/"),
        ::base::TRIM_WHITESPACE, ::base::SPLIT_WANT_NONEMPTY);
    return root()->RepositoryNode::GetNodeFromDirVec(vec);
  } else if (path.type() == ResPath::kRelativePath) {
    return GetLocalNode(path.filepath().as_string());
  } else {
    CHECK(false) << "not support";
    return RepositoryNodePtr();
  }
}

RepositoryNodePtr RepositoryNode::GetRelativeNode(const StringType& path) {
  std::vector<StringType> vec =
      ::base::SplitString(path, FILE_PATH_LITERAL("/"),
                          base::TRIM_WHITESPACE, ::base::SPLIT_WANT_NONEMPTY);
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
    if (parent()->parent())
      path.append(FILE_PATH_LITERAL("/"));
  }

  path.append(name());
  return path;
}

std::string RepositoryNode::PrintHierarchy(int ident) {
  std::stringstream ss;
  std::string ident_str(ident * 2, ' ');
  ss << ident_str << name() << std::endl;

  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    ss << std::move(iter->second->PrintHierarchy(ident + 1));
  }
  return ss.str();
}

void GenerateTreeHierarchy(const ResPath& path, RepositoryNodePtr root) {
  DCHECK_EQ(path.type(), ResPath::kAbsolutePath);
  std::vector<StringType> vec = ::base::SplitString(
      path.filepath().as_string(), FILE_PATH_LITERAL("/"),
      base::TRIM_WHITESPACE, ::base::SPLIT_WANT_NONEMPTY);
  RepositoryNodePtr ptr = root;
  for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
    if (iter->empty()) continue;
    ptr = ptr->FindOrCreate(*iter);
    DCHECK(ptr.get());
  }
}
}  // namespace azer
