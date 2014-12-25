#pragma once

#include "base/basictypes.h"
#include "azer/render/overlay.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DOverlayEffect : public OverlayEffect {
 public:
  D3DOverlayEffect(D3DRenderSystem* rs)
      : OverlayEffect((RenderSystem*)rs) {
  }
 protected:
  bool Init(Overlay* surface, D3DRenderSystem* rs);
  static const char* kVertexShaderProg;
  static const char* kPixelShaderProg;

  virtual void UseTexture(Renderer*) {}
  
  friend class D3DOverlay;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlayEffect);
};

class D3DOverlay : public Overlay {
 public:
  D3DOverlay(const gfx::RectF& rect, D3DRenderSystem* rs)
      : Overlay(rect)
      , render_system_(rs) {}
  virtual ~D3DOverlay() {}
  virtual OverlayEffect* CreateDefaultEffect();

  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  bool Init(azer::RenderSystem* rs);
  bool InitVertex(RenderSystem* rs);
  D3DRenderSystem* render_system_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlay);
};
}  // namespace d3d11
}  // namespace azer
