#pragma once

#include "azer/render/effect_params_provider.h"

namespace azer {
class SceneSurroundings;
class SceneNode;
class SceneEffectParamsProvider : public EffectParamsProvider {
 public:
  SceneEffectParamsProvider() {}
  void SetSceneNode(SceneNode* node) { node_ = node;}
  void SetSceneSurroundings(SceneSurroundings* env) { env_ = env;}
 protected:
  SceneNode* node_;
  SceneSurroundings* env_;
  DISALLOW_COPY_AND_ASSIGN(SceneEffectParamsProvider);
};
}  // namespace azer
