#pragma once

#include "azer/render/effect_params_provider.h"

namespace azer {
class SceneEnvironment;
class SceneNode;
class SceneEffectParamsProvider : public EffectParamsProvider {
 public:
  SceneEffectParamsProvider() {}
  void SetSceneNode(SceneNode* node) { node_ = node;}
  void SetSceneEnvironment(SceneEnvironment* env) { env_ = env;}
 protected:
  SceneNode* node_;
  SceneEnvironment* env_;
  DISALLOW_COPY_AND_ASSIGN(SceneEffectParamsProvider);
};
}  // namespace azer
