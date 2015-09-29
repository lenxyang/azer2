#pragma once

#include "azer/render/render.h"
#include "azer/render/effect_creator.h"
#include "azer/math/math.h"
#include "azer/render/util/light.h"

namespace azer {
class ColoredDiffuseEffect: public Effect {
 public:
  static const char kEffectName[];
  ColoredDiffuseEffect();
  ~ColoredDiffuseEffect();

  const char* name() const override;
  bool Init(const ShaderPrograms& source) override;

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
  };

  struct ps_cbuffer {
    Vector4 color;
    DirLight light;
  };
#pragma pack(pop)

  void SetPVW(const Matrix4& value);
  void SetWorld(const Matrix4& value);
  void SetColor(const Vector4& value);
  void SetDirLight(const DirLight& value);

  struct Vertex {
    Vector4 position;
    Vector4 normal;
  };

  static Effect* CreateObject() {
    return new ColoredDiffuseEffect;
  }
  static const int kVertexDescNum;
  static const VertexDesc::Desc kVertexDesc[];
 protected:
  void InitTechnique(const ShaderPrograms& source);
  void InitGpuConstantTable();
  virtual void UseTexture(Renderer* renderer) override;
  DECLARE_EFFECT_DYNCREATE(ColoredDiffuseEffect);
  DISALLOW_COPY_AND_ASSIGN(ColoredDiffuseEffect);
};


ColoredDiffuseEffect ColoredDiffuseEffect();
}  // namespace azer
