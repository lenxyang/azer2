#pragma once

#include "azer/math/math.h"
#include "azer/render/effect.h"
#include "azer/render/effect_creator.h"

namespace azer {
class NormalLineEffect : public Effect {
 public:
  static const char kEffectName[];
  NormalLineEffect();
  ~NormalLineEffect();

  const char* GetEffectName() const override;

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pv;
    Matrix4 world;
    float linelength;
  };

  struct ps_cbuffer {
    Vector4 color;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetColor(const Vector4& value) {color_ = value;}
  void SetNormalLineLength(float len) { line_length_ = len;}
  static Effect* CreateObject() { return new NormalLineEffect;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override;
  ShaderClosurePtr InitVertexStage(Shader* shader) override;
  ShaderClosurePtr InitGeometryStage(Shader* shader) override;
  ShaderClosurePtr InitPixelStage(Shader* shader) override;


  Matrix4 pv_;
  Matrix4 world_;
  Vector4 color_;
  float line_length_;
  DECLARE_EFFECT_DYNCREATE(NormalLineEffect);
  DISALLOW_COPY_AND_ASSIGN(NormalLineEffect);
};
}  // namespace azer
