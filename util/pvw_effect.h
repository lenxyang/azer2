#pragma once

#include "azer/render/effect.h"
#include "azer/math/math.h"
#include "wow/render/light.h"

namespace azer {
class PVWEffect: public Effect {
 public:
  static const char kEffectName[];
  PVWEffect(VertexDescPtr desc);
  ~PVWEffect();

  const char* name() const override;
  bool Init(const ShaderPrograms& source) override;

  #pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
  };
  #pragma pack(pop)

  void SetPVW(const Matrix4& value);
  void SetWorld(const Matrix4& value);
 protected:
  void UseTexture(Renderer* renderer) override;
  DISALLOW_COPY_AND_ASSIGN(PVWEffect);
};

typedef scoped_refptr<PVWEffect> PVWEffectPtr;

PVWEffectPtr CreateDefaultPVWEffect();
}  // namespace azer
