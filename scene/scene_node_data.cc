#include "azer/scene/scene_node_data.h"

#include "azer/scene/scene_node.h"


namespace azer {
SceneNodeData::SceneNodeData() 
    : type_(kNotSpecified) {
}

void SceneNodeData::Attach(RenderableObjectPtr object) {
  DCHECK_EQ(type(), kNotSpecified);
  renderable_ = object;
  type_ = kRenderableObject;
}

void SceneNodeData::Attach(LightPtr object) {
  DCHECK_EQ(type(), kNotSpecified);
  light_ = object;
  type_ = kLight;
}

void SceneNodeData::Attach(ConfigNodePtr surroundings) {
  DCHECK_EQ(type(), kNotSpecified);
  surroundings_ = surroundings;
  type_ = kSurroundings;
}

void SceneNodeData::Detach() {
  DCHECK_NE(type(), kNotSpecified);
  type_ = kNotSpecified;
  renderable_ = RenderableObjectPtr();
  light_ = LightPtr();
  surroundings_ = ConfigNodePtr();
}
}  // namespace azer
