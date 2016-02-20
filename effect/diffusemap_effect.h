#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/light.h"
#include "azer/effect/material.h"

namespace azer {
class DiffuseMapEffect : public Effect {
 public:
  static const char kEffectName[];
  DiffuseMapEffect();
  ~DiffuseMapEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pv;
    Matrix4 world;
    Vector4 camerapos;
  };

  struct ps_cbuffer {
    float ambient_scalar;
    float specular_scalar;
    float alpha;
    int   light_count;
    UniverseLight lights[4];
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetCameraPos(const Vector4& pos) { camerapos_ = pos;}
  void SetLights(const LightPtr* value, int32 count);
  void SetMaterial(const DiffuseMapMaterialData& mtrl) { mtrl_ = mtrl;}
  static Effect* CreateObject() { return new DiffuseMapEffect;}
 protected:
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 camerapos_;
  int32 light_count_;
  UniverseLight lights_[4];
  DiffuseMapMaterialData mtrl_;
  DECLARE_EFFECT_DYNCREATE(DiffuseMapEffect);
  DISALLOW_COPY_AND_ASSIGN(DiffuseMapEffect);
};


}  // namespace azer
