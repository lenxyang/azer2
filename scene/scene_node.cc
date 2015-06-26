#include "azer/scene/scene_node.h"

#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "base/strings/string_util.h"
#include "base/strings/string_tokenizer.h"
#include "azer/render/render.h"
#include "azer/render/renderable_object.h"
#include "azer/render/frustrum.h"
#include "azer/math/math.h"

namespace azer {
SceneNode::SceneNode() 
    : visible_(false)
    , parent_(NULL)
    , scale_(Vector3(1.0f, 1.0f, 1.0f)) {
  MovableObject::set_delegate(this);
}

SceneNode::SceneNode(const std::string& name)
    : visible_(false)
    , parent_(NULL)
    , scale_(Vector3(1.0f, 1.0f, 1.0f))
    , name_(name) {
  MovableObject::set_delegate(this);
}

SceneNode::~SceneNode() {
}

void SceneNode::AddChild(SceneNodePtr child) {
  DCHECK(child->parent() == NULL);
  child->parent_ = this;
  children_.push_back(child);
}

void SceneNode::RemoveChild(SceneNodePtr child) {
  DCHECK(child->parent_ == this);
  child->parent_ = NULL;

  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if (iter->get() == child.get()) {
      children_.erase(iter);
      break;
    }
  }
}

bool SceneNode::HasAncestor(SceneNode* node) const {
  const SceneNode* cur = node->parent();
  while (cur) {
    if (cur == this) {
      return true;
    }

    cur = cur->parent();
  }

  return false;
}

SceneNode* SceneNode::root() {
  SceneNode* cur = this;
  while (cur) {
    if (cur->parent_ == NULL)
      return cur;
    cur = cur->parent();
  }

  return cur;
}

void SceneNode::OnAttachedToScene() {
}

void SceneNode::OnObjectPositionChanged(const Vector3& origin_position) {
  OnParentNodePositionChanged();
}

void SceneNode::OnObjectOrientationChanged(const Quaternion& origin_orientation) {
  OnParentOrientationChanged();
}

void SceneNode::OnParentNodePositionChanged() {
  UpdateWorldMatrix();
}
void SceneNode::OnParentOrientationChanged() {
  UpdateWorldMatrix();
}

void SceneNode::UpdateWorldMatrix() {
  world_ = std::move(orientation().ToMatrix() * Scale(scale_));
  world_ = std::move(Translate(position()) * world_);
  if (parent()) {
    world_ = std::move(parent()->GetWorldMatrix() * world_);
  }
}

void SceneNode::UpdateWorldMatrixRecusive() {
  UpdateWorldMatrix();
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->UpdateWorldMatrixRecusive();
  }
}

void SceneNode::set_name(const std::string& name) {
  name_ = name;
}

SceneNodePtr SceneNode::GetLocalChild(const std::string& name) {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if ((*iter)->name() == name) {
      return *iter;
    }
  }
  return SceneNodePtr();
}

SceneNodePtr SceneNode::GetChild(const std::string& path) {
  if (StartsWithASCII(path, "//", false)) {
    return root()->GetChild(path.substr(2));
  } else {
    SceneNodePtr cur = this;
    ::base::StringTokenizer t(path, std::string("/"));
    while (t.GetNext()) {
      cur = cur->GetLocalChild(t.token());
      if (!cur.get())
        return SceneNodePtr();
    }
    return cur;
  }
}

void SceneNode::CreatePathRecusive(const std::string& path) {
}

bool SceneNode::AddChildAtPath(const std::string& parent, SceneNodePtr node) {
  SceneNodePtr pnode = GetChild(parent);
  if (pnode.get()) {
    pnode->AddChild(node);
    return true;
  } else {
    return false;
  }
}

SceneNodePtr SceneNode::RemoveChildAtPath(const std::string& path) {
  SceneNodePtr pnode = GetChild(path);
  if (pnode.get() && pnode->parent()) {
    pnode->parent()->RemoveChild(pnode.get());
  }
  return pnode;
}

std::string SceneNode::print_info() {
  std::string str;
  print_info(&str, 0, this);
  return str;
}

void SceneNode::print_info(std::string* str, int depth, SceneNode* node) {
  str->append(std::string(depth, ' '));
  str->append("node[");
  str->append(::base::StringPrintf("name=%s,",name().c_str()));
  str->append(::base::StringPrintf(" pos={%f, %f, %f}", position().x,
                                   position().y, position().z));
  str->append("]\n");
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    (*iter)->print_info(str, depth + 1, iter->get());
  }
}
}  // namespace azer
