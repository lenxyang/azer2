#pragma once

#include "base/basictypes.h"
#include "azer/render/render.h"

namespace azer {

class SceneNode;
class SceneNodeObserver {
 public:
  virtual void OnNodeLocationChanged(SceneNode* n, const Vector3& prev) {}
  virtual void OnNodeOrientChanged(SceneNode* n, const Quaternion& prev) {}
  virtual void OnNodeScaleChanged(SceneNode* node, const Vector3& prev) {}
  virtual void OnNodeBoundsChanged(SceneNode* node, const Vector3& orgmin,
                                   const Vector3& orgmax) {}
};
}  // namespace azer
