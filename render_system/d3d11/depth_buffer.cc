
#include "azer/render_system/d3d11/depth_buffer.h"

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
namespace {
ID3D11Device* GetDevice() {
  RenderSystem* rs  = RenderSystem::Current();
  D3DRenderSystem* d3drs = dynamic_cast<D3DRenderSystem*>(rs);
  DCHECK(d3drs);
  return d3drs->GetDevice();
}
}

// class D3DDepthStencilState
D3DDepthStencilState::D3DDepthStencilState() {
  HRESULT hr;
  D3D11_DEPTH_STENCIL_DESC depth_desc;
  ZeroMemory(&depth_desc, sizeof(depth_desc));
  depth_desc.DepthEnable = TRUE;
  depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
  depth_desc.StencilEnable = FALSE;
  depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
  depth_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depth_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depth_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

  hr = GetDevice()->CreateDepthStencilState(&depth_desc, &depth_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed");
}

D3DDepthStencilState::~D3DDepthStencilState() {
  SAFE_RELEASE(depth_state_);
}

void D3DDepthStencilState::SetDepthState(const D3D11_DEPTH_STENCIL_DESC& desc) {
  DCHECK(depth_state_);
  SAFE_RELEASE(depth_state_);
  HRESULT hr = GetDevice()->CreateDepthStencilState(&desc, &depth_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed ");
}

void D3DDepthStencilState::EnableDepthTest(bool enable) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthEnable = enable;
  return SetDepthState(desc);
}

bool D3DDepthStencilState::IsDepthTestEnabled() {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  return desc.DepthEnable == TRUE;
}

void D3DDepthStencilState::SetDepthCompareFunc(CompareFunc::Type func) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthFunc = TranslateCompareFunc(func);
  SetDepthState(desc);
}

void D3DDepthStencilState::Apply(Renderer* r) {
  D3DRenderer* renderer = dynamic_cast<D3DRenderer*>(r);
  renderer->GetContext()->OMSetDepthStencilState(depth_state_, 0.0f);
}

// class D3DDepthBuffer 
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

void D3DDepthBuffer::Clear(D3DRenderer* renderer, bool clear_depth,
                           bool clear_stencil, float depth_val, int stencil_val) {
  DCHECK(target_ != NULL);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  uint32 flags = 0;
  flags |= (clear_depth ? D3D11_CLEAR_DEPTH : 0);
  flags |= (clear_stencil ? D3D11_CLEAR_STENCIL : 0);
  d3d_context->ClearDepthStencilView(target_,
                                     flags,
                                     depth_val, // depth value (clamp(0.0, 1.0)
                                     stencil_val);   // stencil value
}
}  // namespace d3d11
}  // namespace azer
