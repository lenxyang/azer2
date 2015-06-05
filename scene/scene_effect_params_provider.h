#pragma once

#include "azer/render/effect_params_provider.h"

namespace azer {
class SceneSurroundings;
class SceneNode;
class SceneEffectParamsProvider : public EffectParamsProvider {
 public:
  SceneEffectParamsProvider() {}
  void SetSceneNode(SceneNode* node) { node_ = node;}
  void SetSceneSurroundings(SceneSurroundings* surroundings) { 
    surroundings_ = surroundings;
  }
 protected:
  SceneNode* node_;
  SceneSurroundings* surroundings_;
  DISALLOW_COPY_AND_ASSIGN(SceneEffectParamsProvider);
};
}  // namespace azer
