#include "azer/scene/scene_creator.h"

#include "base/logging.h"

namespace azer {
SceneCreator::SceneCreator() {
}

SceneNodePtr SceneCreator::Create(ConfigNodePtr root) {
  return InitSceneNodeRecusive(root);
}

SceneNodePtr SceneCreator::InitSceneNodeRecusive(ConfigNodePtr config_node) {
  azer::SceneNodePtr node = CreateSceneNode(config_node);
  std::vector<ConfigNodePtr> children = config_node->GetNamedChildren("children");
  if (children.size() == 0u)
    return node;
  if (children.size() > 1u) {
    return SceneNodePtr();
  }

  std::vector<ConfigNodePtr> subnodes = children[0]->GetNamedChildren("node");
  for (auto iter = subnodes.begin(); iter != subnodes.end(); ++iter) {
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
