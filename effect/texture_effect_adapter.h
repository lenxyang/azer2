#pragma once

#include "azer/effect/material.h"
#include "azer/effect/effect_params_adapter.h"

namespace azer {
class SceneNode;
class RenderNode;

class TextureEffect_TextureMaterial_Adapter : public EffectParamsAdapter {
 public:
  TextureEffect_TextureMaterial_Adapter() {}
  EffectAdapterKey key() const override;
  void Apply(Effect* e, const EffectParamsProvider* params) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(TextureEffect_TextureMaterial_Adapter);
};

}  // namespace azer
