#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/light.h"
#include "azer/render/texture.h"

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
    Vector4 bounds;
    Vector4 texbounds;
  };
#pragma pack(pop)

  void SetBounds(const Vector4& v) { bounds_ = v;}
  void SetTexBounds(const Vector4& v) { texbounds_ = v;}
  void SetTexture(Texture* texture) { texture_ = texture;}
  static Effect* CreateObject() { return new OverlayEffect;}
 protected:
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Vector4 bounds_;
  Vector4 texbounds_;
  TexturePtr texture_; 
  DECLARE_EFFECT_DYNCREATE(OverlayEffect);
  DISALLOW_COPY_AND_ASSIGN(OverlayEffect);
};

typedef scoped_refptr<OverlayEffect> OverlayEffectPtr;
}  // namespace azer
