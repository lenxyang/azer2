#include "azer/scene/scene_node_picking.h"

#include "azer/math/math.h"
#include "azer/render/render.h"
#include "azer/scene/scene_node.h"
#include "azer/render/bounding_volumn.h"

namespace azer {
SceneNodePickHelper::SceneNodePickHelper(Ray* ray)
    : ray_(ray),
      picking_node_(NULL) {
}

SceneNodePickHelper::~SceneNodePickHelper() {}

SceneNode* SceneNodePickHelper::GetPickingNode() {
  return picking_node_;
}

void SceneNodePickHelper::OnTraverseBegin(SceneNode* root) {
  worlds_.push(Matrix4::kIdentity);
}

bool SceneNodePickHelper::OnTraverseNodeEnter(SceneNode* node) {
  using namespace azer;
  Vector3 vmin, vmax;
  Matrix4 mat = std::move(worlds_.top() * node->holder().GenWorldMatrix());
  worlds_.push(mat);
  if (picking_node_)
    return false;

  if (!node->visible()) {
    return false;
  }

  if (node->pickable())
    return false;

  vmin = mat * Vector4(node->local_vmin(), 1.0);
  vmax = mat * Vector4(node->local_vmax(), 1.0);  
  AxisAlignedBoundingBox aabb(vmin, vmax);
  if (aabb.IsIntersect(*ray_)) {
    if (node->has_child()) {
      return true;
    } else {
      picking_node_ = node;
      return false;
    }
  } else {
    return false;
  }
}

void SceneNodePickHelper::OnTraverseNodeExit(SceneNode* node) {
  worlds_.pop();
}

void SceneNodePickHelper::OnTraverseEnd() {
  DCHECK_EQ(worlds_.size(), 1u);
}

}  // namespace azer
