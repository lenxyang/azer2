#pragma once

#include "azer/math/math.h"
#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"

namespace azer {
class NormalLineEffect : public Effect {
 public:
  static const char kEffectName[];
  NormalLineEffect();
  ~NormalLineEffect();

  const char* GetEffectName() const override;

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
    float linelength;
  };

  struct ps_cbuffer {
    Vector4 color;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value);
  void SetWorld(const Matrix4& value);
  void SetColor(const Vector4& value);
  void SetNormalLineLength(float len) { line_length_ = len;}
  static Effect* CreateObject() { return new NormalLineEffect;}
 protected:
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 color_;
  float line_length_;
  DECLARE_EFFECT_DYNCREATE(NormalLineEffect);
  DISALLOW_COPY_AND_ASSIGN(NormalLineEffect);
};
}  // namespace azer
