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
    , parent_(NULL) {
  MovableObject::set_delegate(this);
}

SceneNode::~SceneNode() {
}

void SceneNode::AddChild(SceneNodePtr child) {
  DCHECK(child->parent() == NULL);
  children_.push_back(child);
}

void SceneNode::RemoveChild(SceneNodePtr child) {
  child->parent_ = NULL;
  child->root_ = NULL;
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
}

void SceneNode::OnObjectOrientationChanged(const Quaternion& origin_orientation) {
}
}  // namespace azer
