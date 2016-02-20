#pragma once

#include <stack>
#include "azer/render/render.h"
#include "azer/scene/scene_node_traverse.h"

namespace azer {
class SceneNode;
class SceneNodePickHelper : public SceneNodeTraverseDelegate {
 public:
  SceneNodePickHelper(Ray* ray);
  ~SceneNodePickHelper();

  SceneNode* GetPickingNode();
 protected:
  void OnTraverseBegin(SceneNode* root) override;
  bool OnTraverseNodeEnter(SceneNode* node) override;
  void OnTraverseNodeExit(SceneNode* node) override;
  void OnTraverseEnd() override;
 private:
  const Ray* ray_;
  SceneNode* picking_node_;
  std::stack<Matrix4> worlds_;
  DISALLOW_COPY_AND_ASSIGN(SceneNodePickHelper);
};
}  // namespace azer
