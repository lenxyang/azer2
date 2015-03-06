#pragma once

#include "base/basictypes.h"
#include "azer/render/overlay.h"
#include "azer/render/effect.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DOverlayEffect : public Effect {
 public:
  D3DOverlayEffect(D3DRenderSystem* rs)
      : Effect((RenderSystem*)rs) {
  }
  
  void SetTexture(TexturePtr texture) { texture_ = texture;}
  void SetTransform(const Matrix4& matrx);
  void SetTexcoord(const Vector4 texcoord[4]);
  void SetVertex(const Vector4 vertex[4]);  

  void Use(Renderer* renderer) override;

  void UseTexture(Renderer* renderer) override {}
 protected:
  bool Init(Overlay* surface, D3DRenderSystem* rs);
  static const char* kVertexShaderProg;
  static const char* kPixelShaderProg;
  TexturePtr texture_;
  friend class D3DOverlay;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlayEffect);
};

class D3DOverlay : public Overlay {
 public:
  D3DOverlay(D3DRenderSystem* rs)
      : render_system_(rs) {}
  virtual ~D3DOverlay() {}

  void Render(Renderer* renderer) override;

  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  bool Init(azer::RenderSystem* rs);
  bool InitEffect();
  bool InitVertex(RenderSystem* rs);
  D3DRenderSystem* render_system_;
  scoped_refptr<D3DOverlayEffect> effect_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlay);
};
}  // namespace d3d11
}  // namespace azer
