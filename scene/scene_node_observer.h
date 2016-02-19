#pragma once

#include "base/basictypes.h"
namespace azer {
class Quaternion;
class SceneNode;
class Vector3;


class SceneNodeObserver {
 public:
  virtual void OnNodeLocationChanged(SceneNode* n, const Vector3& prev) {}
  virtual void OnNodeOrientChanged(SceneNode* n, const Quaternion& prev) {}
  virtual void OnNodeScaleChanged(SceneNode* node, const Vector3& prev) {}
  virtual void OnNodeBoundsChanged(SceneNode* node, const Vector3& orgmin,
                                   const Vector3& orgmax) {}
};
}  // namespace azer
