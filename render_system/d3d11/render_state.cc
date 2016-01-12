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
  D3D11_RASTERIZER_DESC raster_desc;
  memset(&raster_desc, 0, sizeof(raster_desc));
  raster_desc.FillMode = D3D11_FILL_SOLID;
  raster_desc.CullMode = D3D11_CULL_BACK;
  raster_desc.FrontCounterClockwise = TRUE;
  raster_desc.MultisampleEnable = TRUE;
  raster_desc.AntialiasedLineEnable = TRUE;
  HRESULT hr = GetDevice()->CreateRasterizerState(&raster_desc, &raster_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");

  D3D11_DEPTH_STENCIL_DESC depth_desc;
  ZeroMemory(&depth_desc, sizeof(depth_desc));
  depth_desc.DepthEnable = FALSE;
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

D3DRenderState::~D3DRenderState() {
  SAFE_RELEASE(raster_state_);
  SAFE_RELEASE(depth_state_);
}

FillMode D3DRenderState::GetFillMode(void) {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return TranslateD3DFillMode(desc.FillMode);
}

void D3DRenderState::SetFillMode(FillMode mode) {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  SAFE_RELEASE(raster_state_);
  desc.FillMode = TranslateFillMode(mode);
  SetRasterizerState(desc);
}

CullingMode D3DRenderState::GetCullingMode(void) {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return TranslateD3DCullingMode(desc.CullMode);
}

void D3DRenderState::SetCullingMode(CullingMode mode) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.CullMode = TranslateCullingMode(mode);
  SetRasterizerState(desc);
}

FrontFace D3DRenderState::GetFrontFace() {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  if (desc.FrontCounterClockwise) 
    return kCounterClockwise;
  else 
    return kClockwise;
}

void D3DRenderState::SetFrontFace(FrontFace mode) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.FrontCounterClockwise = (mode == kCounterClockwise);
  SetRasterizerState(desc);
}

void D3DRenderState::EnableMultisampleAntiAliasing(bool enable) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.MultisampleEnable = (enable ? TRUE : FALSE);
  SetRasterizerState(desc);
}

bool D3DRenderState::IsMultisampleAntiAliasingEnabled() {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return desc.MultisampleEnable == TRUE;
} 

void D3DRenderState::EnableLineAntialiasing(bool enable) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.AntialiasedLineEnable = (enable ? TRUE : FALSE);
  SetRasterizerState(desc);
}

bool D3DRenderState::IsLineAntialiasingEnabled() {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return desc.AntialiasedLineEnable == TRUE;
}

void D3DRenderState::SetRasterizerState(const D3D11_RASTERIZER_DESC& desc) {
  DCHECK(raster_state_);
  SAFE_RELEASE(raster_state_);
  HRESULT hr = GetDevice()->CreateRasterizerState(&desc, &raster_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");
}

void D3DRenderState::SetDepthState(const D3D11_DEPTH_STENCIL_DESC& desc) {
  DCHECK(depth_state_);
  SAFE_RELEASE(depth_state_);
  HRESULT hr = GetDevice()->CreateDepthStencilState(&desc, &depth_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed ");
}

void D3DRenderState::Apply(Renderer* r) {
  /*
  D3D11_DEPTH_STENCIL_DESC desc;
  depth_state_->GetDesc(&desc);
  LOG(ERROR) << "Depth Enable: " << (desc.DepthEnable ? "TRUE" : "FALSE");
  */
  D3DRenderer* renderer = dynamic_cast<D3DRenderer*>(r);
  renderer->GetContext()->RSSetState(raster_state_);
  renderer->GetContext()->OMSetDepthStencilState(depth_state_, 0.0f);
}

void D3DRenderState::EnableDepthTest(bool enable) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthEnable = enable;
  return SetDepthState(desc);
}

bool D3DRenderState::IsDepthTestEnabled() {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  return desc.DepthEnable == TRUE;
}

void D3DRenderState::SetDepthCompareFunc(CompareFunc::Type func) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthFunc = TranslateCompareFunc(func);
  SetDepthState(desc);
}
}  // naemspace d3d11
}  // naemspace azer
