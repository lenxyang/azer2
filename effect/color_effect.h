#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/light.h"
#include "azer/effect/material.h"

namespace azer {
class AmbientColorEffect : public Effect {
 public:
  static const char kEffectName[];
  AmbientColorEffect();
  ~AmbientColorEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pv;
    Matrix4 world;
    Vector4 camerapos;
  };

  struct ps_cbuffer {
    Vector4 ambient;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetAmbient(const Vector4& c) { ambient_ = c;}
  static Effect* CreateObject() { return new AmbientColorEffect;}
 protected:
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 ambient_;
  DECLARE_EFFECT_DYNCREATE(AmbientColorEffect);
  DISALLOW_COPY_AND_ASSIGN(AmbientColorEffect);
};


class ColorEffect : public Effect {
 public:
  static const char kEffectName[];
  ColorEffect();
  ~ColorEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pv;
    Matrix4 world;
    Vector4 camerapos;
  };

  struct ps_cbuffer {
    ColorMaterialData mtrl;
    UniverseLight lights[4];
    int    light_count;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value);
  void SetWorld(const Matrix4& value);
  void SetCameraPos(const Vector4& pos);
  void SetLights(const LightPtr* value, int32 count);
  void SetLightData(const UniverseLight* value, int32 count);
  void SetMaterial(const ColorMaterialData& mtrl);
  static Effect* CreateObject() { return new ColorEffect;}
 protected:
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 camerapos_;
  int32 light_count_;
  static const int32 kMaxLightCount = 8;
  UniverseLight lights_[kMaxLightCount];
  ColorMaterialData mtrl_;
  DECLARE_EFFECT_DYNCREATE(ColorEffect);
  DISALLOW_COPY_AND_ASSIGN(ColorEffect);
};

}  // namespace azer
