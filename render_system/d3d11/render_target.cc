#include "azer/render_system/d3d11/render_target.h"

#include <string>

#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/swap_chain.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

D3DRenderTarget::D3DRenderTarget(const Texture::Options& opt,
                                 bool surface_target, 
                                 D3DRenderSystem* render_system)
    : RenderTarget(opt, surface_target)
    , target_(NULL)
    , render_system_(render_system) {
}
  
D3DRenderTarget::~D3DRenderTarget() {
  SAFE_RELEASE(target_);
}

void D3DRenderTarget::Clear(const azer::Vector4& color) {
  DCHECK(NULL != target_);
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->ClearRenderTargetView(
      target_, D3DXCOLOR(color.x, color.y, color.z, color.w));
}

bool D3DRenderTarget::Init() {
  DCHECK(!default_render_target_);
  DCHECK(target_ == NULL);

  ID3D11Device* d3d_device = render_system_->GetDevice();
  HRESULT hr;
  DCHECK(texture_.get() == NULL);
  D3DTexture2D* tex = new D3DTexture2D(options_, render_system_);
  texture_ = tex;
  if (!tex->Init(NULL, 1)) {
    return false;
  }

  ID3D11Resource* resource = ((D3DTexture2D*)texture_.get())->GetResource();
  DCHECK(TranslateBindTarget(options_.target) & D3D11_BIND_RENDER_TARGET);

  hr = d3d_device->CreateRenderTargetView(resource, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");
  return true;
}

D3DRenderTarget* D3DRenderTarget::Create(const Texture::Options& o,
                                         D3DRenderSystem* rs) {
  Texture::Options opt;
  opt = o;
  opt.target = (Texture::BindTarget)(Texture::kRenderTarget | o.target);
  std::unique_ptr<D3DRenderTarget> target(new D3DRenderTarget(opt, false, rs));
  if (!target->Init()) {
    return NULL;
  }

  return target.release();
}

// class D3DCubemapRenderTarget
D3D2DArrayRenderTarget::D3D2DArrayRenderTarget(const TexturePtr texture,
                                               D3DRenderSystem* render_system)
    : RenderTarget(texture->options(), false),
      target_(NULL),
      render_system_(render_system) {
}

D3D2DArrayRenderTarget::~D3D2DArrayRenderTarget() {
}

void D3D2DArrayRenderTarget::Clear(const azer::Vector4& color) {
  DCHECK(NULL != target_);
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->ClearRenderTargetView(
      target_, D3DXCOLOR(color.x, color.y, color.z, color.w));
}

bool D3D2DArrayRenderTarget::Init(int index) {
  DCHECK(!default_render_target_);
  DCHECK(target_ == NULL);
  ID3D11Device* d3d_device = render_system_->GetDevice();

  ID3D11Resource* resource = ((D3DTexture2D*)texture_.get())->GetResource();
  DCHECK(TranslateBindTarget(options_.target) & D3D11_BIND_RENDER_TARGET);

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
  desc.Format = TranslateFormat(texture_->options().format);
  desc.Texture2DArray.ArraySize = 1;
  desc.Texture2DArray.FirstArraySlice = index;
  HRESULT hr = d3d_device->CreateRenderTargetView(resource, &desc, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");
  return true;
}

bool D3D2DArrayRenderTarget::Create(const Texture::Options& o, D3DRenderSystem* rs,
                                    std::vector<RenderTargetPtr>* vec) {
  DCHECK_EQ(o.type, Texture::kCubemap);
  Texture::Options opt;
  opt = o;
  opt.target = (Texture::BindTarget)(Texture::kRenderTarget | o.target);
  std::unique_ptr<D3DTexture2D> tex(new D3DTexture2D(opt, rs));
  if (!tex->Init(NULL, 1)) {
    return false;
  }

  TexturePtr texptr(tex.release());
  for (int i = 0; i < 6; ++i) {
    std::unique_ptr<D3D2DArrayRenderTarget> ptr(new D3D2DArrayRenderTarget(
        texptr, rs));
    if (ptr->Init(i)) {
      vec->push_back(RenderTargetPtr(ptr.release()));
    } else {
      return false;
    }
  }

  return true;
}

// class D3DSurfaceRenderTarget
D3DSurfaceRenderTarget::D3DSurfaceRenderTarget(const Texture::Options& opt,
                                               D3DEnvSwapChain* swapchain,
                                               D3DRenderSystem* rs)
    : D3DRenderTarget(opt, true, rs)
    , swapchain_(swapchain) {
}

bool D3DSurfaceRenderTarget::Init() {
  D3DRenderSystem* rs = render_system_;
  DCHECK(target_ == NULL);
  DCHECK(swapchain_);
  DCHECK(default_render_target_);
  ID3D11Device* d3d_device = rs->GetDevice();
  std::unique_ptr<D3DTexture2D> ptr(new D3DTexture2D(options(), rs));
  HRESULT hr;

  ID3D11Texture2D* buffer = swapchain_->GetSwapTexture();
  ptr->Attach(buffer);
  hr = d3d_device->CreateRenderTargetView(ptr->resource_, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");
  texture_ = ptr.release();
  return true;
}

D3DRenderTarget* D3DSurfaceRenderTarget::Create(D3DEnvSwapChain* swapchain,
                                                D3DRenderSystem* rs) {
  Surface* surface = swapchain->GetSurface();
  Texture::Options opt;
  opt.size = gfx::Size(surface->GetBounds().size());
  opt.target = (Texture::BindTarget)(Texture::kRenderTarget);
  std::unique_ptr<D3DSurfaceRenderTarget> target(
      new D3DSurfaceRenderTarget(opt, swapchain, rs));
  if (!target->Init()) {
    return NULL;
  }

  return target.release();
  
}
}  // namespace d3d11
}  // namespace azer
