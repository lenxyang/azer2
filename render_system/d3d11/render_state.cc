#include "azer/render_system/d3d11/render_state.h"

#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/enum_transform.h"

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

D3DRenderState::D3DRenderState() {
  D3D11_RASTERIZER_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_BACK;
  desc.FrontCounterClockwise = TRUE;
  desc.MultisampleEnable = TRUE;
  desc.AntialiasedLineEnable = TRUE;
  HRESULT hr = GetDevice()->CreateRasterizerState(&desc, &state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");
}

D3DRenderState::~D3DRenderState() {
  SAFE_RELEASE(state_);
}

FillMode D3DRenderState::GetFillMode(void) {
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  return TranslateD3DFillMode(desc.FillMode);
}

void D3DRenderState::SetFillMode(FillMode mode) {
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  SAFE_RELEASE(state_);
  desc.FillMode = TranslateFillMode(mode);
  SetRasterizerState(desc);
}

CullingMode D3DRenderState::GetCullingMode(void) {
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  return TranslateD3DCullingMode(desc.CullMode);
}

void D3DRenderState::SetCullingMode(CullingMode mode) {
  DCHECK(state_);
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  desc.CullMode = TranslateCullingMode(mode);
  SetRasterizerState(desc);
}

FrontFace D3DRenderState::GetFrontFace() {
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  if (desc.FrontCounterClockwise) 
    return kCounterClockwise;
  else 
    return kClockwise;
}

void D3DRenderState::SetFrontFace(FrontFace mode) {
  DCHECK(state_);
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  desc.FrontCounterClockwise = (mode == kCounterClockwise);
  SetRasterizerState(desc);
}

void D3DRenderState::EnableMultisampleAntiAliasing(bool enable) {
  DCHECK(state_);
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  desc.MultisampleEnable = (enable ? TRUE : FALSE);
  SetRasterizerState(desc);
}

bool D3DRenderState::IsMultisampleAntiAliasingEnabled() {
  DCHECK(state_);
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  return desc.MultisampleEnable == TRUE;
} 

void D3DRenderState::EnableLineAntialiasing(bool enable) {
  DCHECK(state_);
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  desc.AntialiasedLineEnable = (enable ? TRUE : FALSE);
  SetRasterizerState(desc);
}

bool D3DRenderState::IsLineAntialiasingEnabled() {
  DCHECK(state_);
  D3D11_RASTERIZER_DESC desc;
  state_->GetDesc(&desc);
  return desc.AntialiasedLineEnable == TRUE;
}

void D3DRenderState::SetRasterizerState(const D3D11_RASTERIZER_DESC& desc) {
  DCHECK(state_);
  SAFE_RELEASE(state_);
  HRESULT hr = GetDevice()->CreateRasterizerState(&desc, &state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");
}

void D3DRenderState::Apply(Renderer* r) {
  D3DRenderer* renderer = dynamic_cast<D3DRenderer*>(r);
  renderer->GetContext()->RSSetState(state_);
}
}  // naemspace d3d11
}  // naemspace azer
