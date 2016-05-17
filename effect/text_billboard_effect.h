#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/light.h"
#include "azer/effect/material.h"

namespace azer {
class TextBillboardEffect : public Effect {
 public:
  static const char kEffectName[];
  TextBillboardEffect();
  ~TextBillboardEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 world;
  };

  struct gs_cbuffer {
    Matrix4 pv;
    float4  viewup;
    float4  viewpos;
    float4  param;
  };

  struct ps_cbuffer {
    Vector4 diffuse;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetViewPos(const Vector3& v) { viewpos_ = v;}
  void SetViewUp(const Vector3& v) { viewup_ = v;}
  void SetBillboard(float width, float height);
  void SetTexture(TextureView* tex);
  void SetDiffuse(const Vector4& v) { diffuse_ = v;}
  static Effect* CreateObject() { return new TextBillboardEffect;}
 protected:
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector3 viewpos_;
  Vector3 viewup_;
  Vector2 param_;
  Vector4 diffuse_;
  TextureViewPtr tex_;
  DECLARE_EFFECT_DYNCREATE(TextBillboardEffect);
  DISALLOW_COPY_AND_ASSIGN(TextBillboardEffect);
};


}  // namespace azer
