#include "azer/scene/node.h"

#include "azer/scene/scene.h"
#include "azer/render/render.h"
#include "azer/render/frustrum.h"
#include "azer/math/math.h"

namespace azer {
bool SceneNode::IsVisible(const Frustrum& frustrum) {
  return true;
}
}  // namespace azer
