#pragma once

#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_creator.h"
#include "azer/render/vertex_buffer.h"
#include "azer/math/math.h"
#include "azer/render/util/light.h"

namespace azer {
class ColoredDiffuseEffect : public Effect {
 public:
  static const char kEffectName[];
  ColoredDiffuseEffect(VertexDescPtr desc);
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
 protected:
  void InitTechnique(const ShaderPrograms& source);
  void InitGpuConstantTable();
  virtual void UseTexture(Renderer* renderer) override;
  DISALLOW_COPY_AND_ASSIGN(ColoredDiffuseEffect);
};

typedef scoped_refptr<ColoredDiffuseEffect> ColoredDiffuseEffectPtr;
ColoredDiffuseEffectPtr CreateColoredDiffuseEffect();
}  // namespace azer
