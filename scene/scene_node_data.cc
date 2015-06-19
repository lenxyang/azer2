#include "azer/scene/scene_node_data.h"

#include "azer/scene/scene_node.h"


namespace azer {
SceneNodeData::SceneNodeData() {
}

void SceneNodeData::AttachRenderable(RenderableObjectPtr object) {
  renderable_ = object;
}

void SceneNodeData::AttachLight(LightPtr object) {
  light_ = object;
}

void SceneNodeData::AttachConfig(ConfigNodePtr surroundings) {
  surroundings_ = surroundings;
}

void SceneNodeData::AttachSky(SkyPtr object) {
  sky_ = object;
}

void SceneNodeData::DetachAll() {
  renderable_ = RenderableObjectPtr();
  light_ = LightPtr();
  surroundings_ = ConfigNodePtr();
  sky_ = SkyPtr();
}
}  // namespace azer
