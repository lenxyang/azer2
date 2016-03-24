#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
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
  void SetTexture(Texture* texture) { cubemap_ = texture;}
  static Effect* CreateObject() { return new SkyboxEffect;}
 protected:
  void BindTexture(int32 mode, Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  TexturePtr cubemap_;
  DECLARE_EFFECT_DYNCREATE(SkyboxEffect);
  DISALLOW_COPY_AND_ASSIGN(SkyboxEffect);
};

}  // namespace azer
