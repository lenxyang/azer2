#pragma once

#include "azer/render/effect.h"
#include "azer/render/effect_creator.h"
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

  void SetPV(const Matrix4& value);
  void SetWorld(const Matrix4& value);
  void SetCameraPos(const Vector4& pos);
  void SetLights(const LightPtr* value, int32_t count);
  void SetLightData(const UniverseLight* value, int32_t count);
  void SetMaterial(const DiffuseMapMaterialData& mtrl);
  static Effect* CreateObject() { return new DiffuseMapEffect;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override;
  ShaderClosurePtr InitVertexStage(Shader* shader) override;
  ShaderClosurePtr InitPixelStage(Shader* shader) override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 camerapos_;
  int32_t light_count_;
  static const int32_t kMaxLightCount = 8;
  UniverseLight lights_[kMaxLightCount];
  DiffuseMapMaterialData mtrl_;
  DECLARE_EFFECT_DYNCREATE(DiffuseMapEffect);
  DISALLOW_COPY_AND_ASSIGN(DiffuseMapEffect);
};


}  // namespace azer
