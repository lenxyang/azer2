#pragma once

#include "azer/effect/material.h"
#include "azer/effect/effect_params_adapter.h"

namespace azer {
class SceneNode;
class RenderNode;

class DiffuseMapEffect_DiffuseMapMaterial_Adapter : public EffectParamsAdapter {
 public:
  DiffuseMapEffect_DiffuseMapMaterial_Adapter() {}
  EffectAdapterKey key() const override;
  void Apply(Effect* e, const EffectParamsProvider* params) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(DiffuseMapEffect_DiffuseMapMaterial_Adapter);
};

class DiffuseMapEffect_RenderNode_Adapter : public EffectParamsAdapter {
 public:
  DiffuseMapEffect_RenderNode_Adapter() {}

  EffectAdapterKey key() const override;
  void Apply(Effect* e, const EffectParamsProvider* params) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(DiffuseMapEffect_RenderNode_Adapter);
};

}  // namespace azer
