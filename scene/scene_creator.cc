#include "azer/scene/scene_creator.h"

namespace azer {
SceneCreator::SceneCreator() {
}

SceneNodePtr SceneCreator::Create(ConfigNodePtr root) {
  return InitSceneNodeRecusive(root);
}

SceneNodePtr SceneCreator::InitSceneNodeRecusive(ConfigNodePtr config_node) {
  azer::SceneNodePtr node = CreateSceneNode(config_node);
  std::vector<ConfigNodePtr> children = config_node->GetNamedChildren("node");
  for (auto iter = children.begin(); iter != children.end(); ++iter) {
    azer::ConfigNodePtr child_node = *iter;
    azer::SceneNodePtr child_scene = InitSceneNodeRecusive(child_node);
    if (child_scene.get()) {
      if (InitSceneConfig(child_scene, config_node))
        node->AddChild(child_scene);
      else
        return SceneNodePtr();
    } else {
      return SceneNodePtr();
    }
  }

  return node;
}

bool SceneCreator::InitSceneConfig(SceneNodePtr scene, ConfigNodePtr config) {
  return true;
}
}  // namespace azer
