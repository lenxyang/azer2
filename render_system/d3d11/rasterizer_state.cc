#include "azer/render_system/d3d11/rasterizer_state.h"

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

D3DRasterizerState::D3DRasterizerState()
    : raster_state_(NULL) {
  D3D11_RASTERIZER_DESC raster_desc;
  memset(&raster_desc, 0, sizeof(raster_desc));
  raster_desc.FillMode = D3D11_FILL_SOLID;
  raster_desc.CullMode = D3D11_CULL_BACK;
  raster_desc.FrontCounterClockwise = TRUE;
  raster_desc.MultisampleEnable = TRUE;
  raster_desc.AntialiasedLineEnable = TRUE;
  HRESULT hr = GetDevice()->CreateRasterizerState(&raster_desc, &raster_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");
}

D3DRasterizerState::~D3DRasterizerState() {
  SAFE_RELEASE(raster_state_);
}

FillMode D3DRasterizerState::GetFillMode(void) {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return TranslateD3DFillMode(desc.FillMode);
}

void D3DRasterizerState::SetFillMode(FillMode mode) {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.FillMode = TranslateFillMode(mode);
  SetRasterizerState(desc);
}

CullingMode D3DRasterizerState::GetCullingMode(void) {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return TranslateD3DCullingMode(desc.CullMode);
}

void D3DRasterizerState::SetCullingMode(CullingMode mode) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.CullMode = TranslateCullingMode(mode);
  SetRasterizerState(desc);
}

FrontFace D3DRasterizerState::GetFrontFace() {
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  if (desc.FrontCounterClockwise) 
    return kCounterClockwise;
  else 
    return kClockwise;
}

void D3DRasterizerState::SetFrontFace(FrontFace mode) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.FrontCounterClockwise = (mode == kCounterClockwise);
  SetRasterizerState(desc);
}

void D3DRasterizerState::EnableMultisampleAntiAliasing(bool enable) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.MultisampleEnable = (enable ? TRUE : FALSE);
  SetRasterizerState(desc);
}

bool D3DRasterizerState::IsMultisampleAntiAliasingEnabled() {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return desc.MultisampleEnable == TRUE;
} 

void D3DRasterizerState::EnableLineAntialiasing(bool enable) {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  desc.AntialiasedLineEnable = (enable ? TRUE : FALSE);
  SetRasterizerState(desc);
}

bool D3DRasterizerState::IsLineAntialiasingEnabled() {
  DCHECK(raster_state_);
  D3D11_RASTERIZER_DESC desc;
  raster_state_->GetDesc(&desc);
  return desc.AntialiasedLineEnable == TRUE;
}

void D3DRasterizerState::SetRasterizerState(const D3D11_RASTERIZER_DESC& desc) {
  DCHECK(raster_state_);
  SAFE_RELEASE(raster_state_);
  HRESULT hr = GetDevice()->CreateRasterizerState(&desc, &raster_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");
}

void D3DRasterizerState::Apply(Renderer* r) {
  /*
  D3D11_DEPTH_STENCIL_DESC desc;
  depth_state_->GetDesc(&desc);
  LOG(ERROR) << "Depth Enable: " << (desc.DepthEnable ? "TRUE" : "FALSE");
  */
  D3DRenderer* renderer = dynamic_cast<D3DRenderer*>(r);
  renderer->GetContext()->RSSetState(raster_state_);
}
}  // naemspace d3d11
}  // naemspace azer
