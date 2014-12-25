#pragma once

#include <d3d11.h>
#include "azer/render/render_target.h"
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/swap_chain.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;
class D3DSurfaceRenderTarget;

class D3DRenderTarget : public RenderTarget {
 public:
  // create by RenderSystem
  static D3DRenderTarget* Create(const Texture::Options& opt,
                                 D3DRenderer* renderer);
  D3DRenderTarget(const Texture::Options& opt,
                  bool surface_target, 
                  D3DRenderer* renderer)
      : RenderTarget(opt, surface_target)
      , target_(NULL)
      , renderer_(renderer) {
  }
  
  virtual ~D3DRenderTarget() {
    SAFE_RELEASE(target_);
  }
  
  virtual void Clear(const azer::Vector4& color);

  bool Init(D3DRenderSystem* rs);
  ID3D11RenderTargetView* GetD3DRenderTargetView() { return target_;}
 protected:
  ID3D11RenderTargetView* target_;
  D3DRenderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderTarget);
};

class D3DSurfaceRenderTarget : public D3DRenderTarget {
 public:
  static D3DSurfaceRenderTarget* Create(Surface* surface, D3DRenderer* r);

  D3DSurfaceRenderTarget(const Texture::Options& opt, Surface* surface,
                         D3DRenderer* renderer)
      : D3DRenderTarget(opt, true, renderer)
      , surface_(surface) {
  }

  bool Init();
 protected:
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3DSurfaceRenderTarget);
};

}  // namespace d3d11
}  // namespace azer
