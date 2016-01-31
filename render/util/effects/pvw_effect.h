#pragma once

#include "azer/render/effect.h"
#include "azer/math/math.h"
#include "azer/render/util/light.h"

namespace azer {
class PVWEffect: public Effect {
 public:
  static const char kEffectName[];
  PVWEffect();
  ~PVWEffect();

  const char* GetEffectName() const override;
  bool Init(VertexDesc* desc, const Shaders& source) override;

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
  };
#pragma pack(pop)

  void SetPVW(const Matrix4& value);
  void SetWorld(const Matrix4& value);
 protected:
  void ApplyGpuConstantTable(Renderer* renderer) override;
  vs_cbuffer data_;
  DISALLOW_COPY_AND_ASSIGN(PVWEffect);
};

typedef scoped_refptr<PVWEffect> PVWEffectPtr;

PVWEffectPtr CreatePVWEffect();
}  // namespace azer
