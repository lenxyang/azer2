#include "azer/scene/scene_node_data.h"

#include "azer/scene/scene_node.h"


namespace azer {
SceneNodeData::SceneNodeData() {
}

void SceneNodeData::Attach(RenderableObjectPtr object) {
  renderable_ = object;
}

void SceneNodeData::Attach(LightPtr object) {
  light_ = object;
}

void SceneNodeData::Attach(ConfigNodePtr surroundings) {
  surroundings_ = surroundings;
}

void SceneNodeData::DetachAll() {
  renderable_ = RenderableObjectPtr();
  light_ = LightPtr();
  surroundings_ = ConfigNodePtr();
}
}  // namespace azer
