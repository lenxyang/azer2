#include "azer/render_system/d3d11/render_target.h"

#include <string>

#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/swap_chain.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {

void D3DRenderTarget::Clear(const azer::Vector4& color) {
  DCHECK(NULL != target_);
  DCHECK(NULL != renderer_);
  ID3D11DeviceContext* d3d_context = renderer_->GetContext();
  d3d_context->ClearRenderTargetView(
      target_, D3DXCOLOR(color.x, color.y, color.z, color.w));
}

bool D3DRenderTarget::Init(D3DRenderSystem* rs) {
  DCHECK(!default_render_target_);
  DCHECK(target_ == NULL);

  ID3D11Device* d3d_device = rs->GetDevice();
  HRESULT hr;
  DCHECK(texture_.get() == NULL);
  D3DTexture2D* tex = new D3DTexture2D(options_, rs);
  texture_.reset(tex);
  if (!tex->Init(NULL, 1)) {
    return false;
  }

  ID3D11Resource* resource = ((D3DTexture2D*)texture_.get())->resource_;
  DCHECK(TranslateBindTarget(options_.target) & D3D11_BIND_RENDER_TARGET);

  hr = d3d_device->CreateRenderTargetView(resource, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");
  return true;
}

bool D3DSurfaceRenderTarget::Init() {
  D3DRenderSystem* rs = (D3DRenderSystem*)renderer_->GetRenderSystem();
  DCHECK(target_ == NULL);
  DCHECK(default_render_target_);
  ID3D11Device* d3d_device = renderer_->GetDevice();
  std::unique_ptr<D3DTexture2D> ptr(new D3DTexture2D(options(), rs));
  HRESULT hr;
  
  ID3D11Texture2D* buffer = rs->GetD3DEnv()->GetSwapTexture();
  ptr->Attach(buffer);
  hr = d3d_device->CreateRenderTargetView(ptr->resource_, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");
  texture_.reset(ptr.release());
  return true;
}

D3DRenderTarget* D3DRenderTarget::Create(const Texture::Options& o,
                                         D3DRenderer* r) {
  D3DRenderSystem* rs = (D3DRenderSystem*)r->GetRenderSystem();
  Texture::Options opt;
  opt = o;
  opt.target = (Texture::BindTarget)(Texture::kRenderTarget | o.target);
  std::unique_ptr<D3DRenderTarget> target(new D3DRenderTarget(opt, false, r));
  if (!target->Init(rs)) {
    return NULL;
  }

  return target.release();
}

D3DSurfaceRenderTarget* D3DSurfaceRenderTarget::Create(Surface* surface,
                                                       D3DRenderer* r) {
  Texture::Options opt;
  opt.size = surface->GetBounds().size();
  opt.target = Texture::kRenderTarget;

  std::unique_ptr<D3DSurfaceRenderTarget> target(
      new D3DSurfaceRenderTarget(opt, surface, r));
  if (!target->Init()) {
    return NULL;
  }
  
  return target.release();
}
}  // namespace d3d11
}  // namespace azer
