#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/light.h"

namespace azer {
class Texture;

class OverlayEffect : public Effect {
 public:
  static const char kEffectName[];
  OverlayEffect();
  ~OverlayEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    float left;
    float top;
    float width;
    float height;
  };
#pragma pack(pop)

  void SetParams(const Vector4& params) { params_ = params;}
  void SetTexture(Texture* texture) { texture_ = texture;}
  static Effect* CreateObject() { return new OverlayEffect;}
 protected:
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Vector4 params_;
  TexturePtr texture_; 
  DECLARE_EFFECT_DYNCREATE(OverlayEffect);
  DISALLOW_COPY_AND_ASSIGN(OverlayEffect);
};

typedef scoped_refptr<OverlayEffect> OverlayEffectPtr;
}  // namespace azer
