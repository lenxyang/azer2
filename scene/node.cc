#include "azer/scene/node.h"

#include "base/logging.h"
#include "azer/render/render.h"
#include "azer/render/renderable_object.h"
#include "azer/render/frustrum.h"
#include "azer/math/math.h"

namespace azer {
SceneNode::SceneNode() 
    : visible_(false)
    , root_(NULL)
    , parent_(NULL)
    , scale_(Vector3(1.0f, 1.0f, 1.0f)) {
  MovableObject::set_delegate(this);
}

SceneNode::~SceneNode() {
}

void SceneNode::AddChild(SceneNodePtr child) {
  DCHECK(child->parent() == NULL);
  children_.push_back(child);
}

void SceneNode::RemoveChild(SceneNodePtr child) {
  DCHECK(child->parent_ == this);
  child->parent_ = NULL;
  child->root_ = NULL;

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
  world_ = std::move(orientation().ToMatrix() * azer::Scale(scale_));
  world_ = std::move(azer::Translate(position()) * world_);
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
}  // namespace azer
