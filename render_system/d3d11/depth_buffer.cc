
#include "azer/render_system/d3d11/depth_buffer.h"

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {

D3DDepthBuffer* D3DDepthBuffer::Create(const Texture::Options& o, 
                                       D3DRenderer* renderer) {
  Texture::Options opt;
  opt = o;
  opt.format = kDepth24Stencil8;
  opt.target = Texture::kDepthStencil;
  std::unique_ptr<D3DDepthBuffer> ptr(new D3DDepthBuffer(opt, renderer));
  if (!ptr->Init((D3DRenderSystem*)renderer->GetRenderSystem())) {
    return NULL;
  }

  return ptr.release();
}

D3DDepthBuffer* D3DDepthBuffer::Create(Surface* surface,
                                       D3DRenderer* renderer) {
  Texture::Options o;
  o.width = surface->GetBounds().width();
  o.height = surface->GetBounds().height();
  return Create(o, renderer);
}

bool D3DDepthBuffer::InitDepthAndStencilState(D3DRenderSystem* rs) {
  HRESULT hr;
  ID3D11DepthStencilState* state;
  ID3D11Device* d3d_device = rs->GetDevice();
  ZeroMemory(&desc_, sizeof(desc_));
  desc_.DepthEnable = FALSE;
  desc_.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  desc_.DepthFunc = D3D11_COMPARISON_LESS;
  desc_.StencilEnable = FALSE;
  desc_.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  desc_.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
  desc_.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  desc_.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc_.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc_.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  desc_.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  desc_.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  desc_.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  desc_.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

  hr = d3d_device->CreateDepthStencilState(&desc_, &state);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed");
  SAFE_RELEASE(state);
  return true;
}

void D3DDepthBuffer::PushState() {
  state_stack_.push(desc_);
}

void D3DDepthBuffer::PopState() {
  DCHECK(!state_stack_.empty());
  desc_ = state_stack_.top();
  state_stack_.pop();
  UpdateState();
}

void D3DDepthBuffer::Enable(bool enable) {
  desc_.DepthEnable = enable;
  UpdateState();
}

void D3DDepthBuffer::UpdateState() {
  ID3D11DeviceContext* d3d_context = renderer_->GetContext();

  ID3D11DepthStencilState* state = NULL;
  ID3D11Device* d3d_device = renderer_->GetDevice();
  HRESULT hr = d3d_device->CreateDepthStencilState(&desc_, &state);
  
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed");
  d3d_context->OMSetDepthStencilState(state, stencil_ref_value_);
  SAFE_RELEASE(state);
}

bool D3DDepthBuffer::IsEnabled() {
  return desc_.DepthEnable == TRUE;
}

void D3DDepthBuffer::SetDepthCompareFunc(CompareFunc::Type func) {
  desc_.DepthFunc = TranslateCompareFunc(func);
  UpdateState();
}

bool D3DDepthBuffer::Init(D3DRenderSystem* rs) {
  ID3D11Device* d3d_device = rs->GetDevice();
  HRESULT hr;
  DCHECK(texture_.get() == NULL);
  D3DTexture2D* tex = new D3DTexture2D(options_, rs);
  texture_.reset(tex);
  if (!tex->Init(NULL, 1)) {
    return false;
  }

  ID3D11Resource* resource = ((D3DTexture2D*)texture_.get())->resource_;
  DCHECK_EQ(TranslateBindTarget(options_.target), D3D11_BIND_DEPTH_STENCIL);

  hr = d3d_device->CreateDepthStencilView(resource, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateDepthStencilView failed ");  

  return InitDepthAndStencilState(rs);
}

void D3DDepthBuffer::Clear(D3DRenderer* renderer, ClearFlag flag,
                           float depth_val, int stencil_val) {
  DCHECK(target_ != NULL);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->ClearDepthStencilView(target_,
                                     TranslateDepthAndStencilClearFlag(flag),
                                     depth_val, // depth value (clamp(0.0, 1.0)
                                     stencil_val);   // stencil value
  stencil_ref_value_ = stencil_val;
}
}  // namespace d3d11
}  // namespace azer
