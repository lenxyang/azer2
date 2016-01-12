
#include "azer/render_system/d3d11/depth_buffer.h"

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

D3DDepthBuffer::D3DDepthBuffer(const Texture::Options& opt, D3DRenderSystem* rs)
    : DepthBuffer(opt),
      target_(NULL),
      render_system_(rs) {
  }

D3DDepthBuffer::~D3DDepthBuffer() {
  SAFE_RELEASE(target_);
}

D3DDepthBuffer* D3DDepthBuffer::Create(const Texture::Options& o, 
                                       D3DRenderSystem* rs) {
  Texture::Options opt;
  opt = o;
  opt.format = kDepth24Stencil8;
  opt.target = Texture::kDepthStencil;
  opt.genmipmap = false;
  std::unique_ptr<D3DDepthBuffer> ptr(new D3DDepthBuffer(opt, rs));
  if (!ptr->Init(rs)) {
    return NULL;
  }

  return ptr.release();
}

D3DDepthBuffer* D3DDepthBuffer::Create(Surface* surface, D3DRenderSystem* rs) {
  Texture::Options o;
  o.size = surface->GetBounds().size();
  o.sampler.sample_level = surface->sample_count();
  o.sampler.sample_quality = surface->sample_quality();
  return Create(o, rs);
}

bool D3DDepthBuffer::Init(D3DRenderSystem* rs) {
  ID3D11Device* d3d_device = rs->GetDevice();
  HRESULT hr;
  DCHECK(texture_.get() == NULL);
  D3DTexture2D* tex = new D3DTexture2D(options_, rs);
  texture_ = tex;
  if (!tex->Init(NULL, 1)) {
    return false;
  }

  ID3D11Resource* resource = ((D3DTexture2D*)texture_.get())->resource_;
  DCHECK_EQ(TranslateBindTarget(options_.target), D3D11_BIND_DEPTH_STENCIL);

  hr = d3d_device->CreateDepthStencilView(resource, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateDepthStencilView failed ");  

  return true;
}

void D3DDepthBuffer::Clear(D3DRenderer* renderer, ClearFlag flag,
                           float depth_val, int stencil_val) {
  DCHECK(target_ != NULL);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->ClearDepthStencilView(target_,
                                     TranslateDepthAndStencilClearFlag(flag),
                                     depth_val, // depth value (clamp(0.0, 1.0)
                                     stencil_val);   // stencil value
}
}  // namespace d3d11
}  // namespace azer
