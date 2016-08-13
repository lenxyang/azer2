#pragma once

#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/light.h"
#include "azer/render/texture.h"
#include "azer/render/texture_view.h"

namespace azer {
class Resource;

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
  void SetTexture(TextureView* texture);
  static Effect* CreateObject() { return new OverlayEffect;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override;
  ShaderClosurePtr InitVertexStage(Shader* shader) override;
  ShaderClosurePtr InitPixelStage(Shader* shader) override;

  Vector4 bounds_;
  Vector4 texbounds_;
  TextureViewPtr texture_; 
  DECLARE_EFFECT_DYNCREATE(OverlayEffect);
  DISALLOW_COPY_AND_ASSIGN(OverlayEffect);
};

typedef scoped_refptr<OverlayEffect> OverlayEffectPtr;
}  // namespace azer
