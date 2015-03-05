#include "azer/scene/node.h"

#include "azer/render/render.h"
#include "azer/render/frustrum.h"
#include "azer/math/math.h"

namespace azer {
SceneNode::SceneNode() {
  MovableObject::set_delegate(this);
  TreeNode<SceneNode>::set_delegate(this);
}

SceneNode::~SceneNode() {
  MovableObject::set_delegate(NULL);
  TreeNode<SceneNode>::set_delegate(NULL);
}

void SceneNode::OnObjectPositionChanged(const Vector3& origin_position) {
}

void SceneNode::OnObjectOrientationChanged(const Quaternion& origin_orientation) {
}

void SceneNode::OnChildAdded(SceneNode* child) {
}

void SceneNode::OnChildRemoved(SceneNode* child) {
}
}  // namespace azer
