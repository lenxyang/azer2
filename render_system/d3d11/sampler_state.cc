#include "azer/render_system/d3d11/sampler_state.h"

#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
D3DSamplerState::D3DSamplerState(const Options& opt, D3DRenderSystem* rs)
    : SamplerState(opt), 
      sampler_state_(NULL),
      render_system_(rs) {
}

D3DSamplerState::~D3DSamplerState() {
  SAFE_RELEASE(sampler_state_);
}

bool D3DSamplerState::Init() {
  ID3D11Device* d3d_device = render_system_->GetDevice();
  if (sampler_state_) {
    SAFE_RELEASE(sampler_state_);
  }
  D3D11_SAMPLER_DESC sampler_desc;
  ZeroMemory(&sampler_desc, sizeof(sampler_desc));
  sampler_desc.Filter = TranslateSamplerState(sampler_state);
  sampler_desc.AddressU = TranslateTexWrapMode(sampler_state.wrap_u);
  sampler_desc.AddressV = TranslateTexWrapMode(sampler_state.wrap_v);
  sampler_desc.AddressW = TranslateTexWrapMode(sampler_state.wrap_w);
  sampler_desc.ComparisonFunc = TranslateCompareFunc(sampler_state.compare_func);
  sampler_desc.MaxAnisotropy = sampler_state.max_anisotropy;
  sampler_desc.MipLODBias = 0.0f;
  sampler_desc.MinLOD = 0;
  sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
  sampler_desc.BorderColor[0] = sampler_state.border_color.x;
  sampler_desc.BorderColor[1] = sampler_state.border_color.y;
  sampler_desc.BorderColor[2] = sampler_state.border_color.z;
  sampler_desc.BorderColor[3] = sampler_state.border_color.w;
  HRESULT hr = d3d_device->CreateSamplerState(&sampler_desc, &sampler_state_);
  HRESULT_HANDLE(hr, ERROR, "CreateSamplerState failed ");

  D3D11_SAMPLER_DESC sampler_desc2;
  sampler_state_->GetDesc(&sampler_desc2);
}
}  // namespace d3d11
}  // namespace azer
