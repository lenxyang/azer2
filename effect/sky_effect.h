#pragma once

#include "azer/render/effect.h"
#include "azer/render/effect_creator.h"
#include "azer/effect/light.h"
#include "azer/effect/material.h"

namespace azer {
class SkyboxEffect : public Effect {
 public:
  static const char kEffectName[];
  SkyboxEffect();
  ~SkyboxEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pv;
    Matrix4 world;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetTexture(TextureView* texture);
  static Effect* CreateObject() { return new SkyboxEffect;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override;
  ShaderClosurePtr InitVertexStage(Shader* shader) override;
  ShaderClosurePtr InitPixelStage(Shader* shader) override;

  Matrix4 pv_;
  Matrix4 world_;
  TextureViewPtr cubemap_;
  DECLARE_EFFECT_DYNCREATE(SkyboxEffect);
  DISALLOW_COPY_AND_ASSIGN(SkyboxEffect);
};

}  // namespace azer
