#pragma once

#include <d3d11.h>
#include "azer/render/render_target.h"
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/swap_chain.h"

namespace azer {
class D3D11RenderSystem;
class D3D11Renderer;
class D3D11SurfaceRenderTarget;

class D3D11RenderTarget : public RenderTarget {
 public:
  // create by RenderSystem
  static D3D11RenderTarget* Create(const Texture::Options& opt,
                                   D3D11Renderer* renderer);
  D3D11RenderTarget(const Texture::Options& opt,
                    bool surface_target, 
                    D3D11Renderer* renderer)
      : RenderTarget(opt, surface_target)
      , target_(NULL)
      , renderer_(renderer) {
  }
  
  virtual ~D3D11RenderTarget() {
    SAFE_RELEASE(target_);
  }
  
  virtual void Clear(const azer::Vector4& color);

  bool Init(D3D11RenderSystem* rs);
  ID3D11RenderTargetView* GetD3D11RenderTargetView() { return target_;}
 protected:
  ID3D11RenderTargetView* target_;
  D3D11Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(D3D11RenderTarget);
};

class D3D11SurfaceRenderTarget : public D3D11RenderTarget {
 public:
  static D3D11SurfaceRenderTarget* Create(Surface* surface, D3D11Renderer* r);

  D3D11SurfaceRenderTarget(const Texture::Options& opt, Surface* surface,
                           D3D11Renderer* renderer)
      : D3D11RenderTarget(opt, true, renderer)
      , surface_(surface) {
  }

  bool Init();
 protected:
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3D11SurfaceRenderTarget);
};

}  // namespace azer
