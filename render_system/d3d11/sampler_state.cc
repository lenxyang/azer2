#include "azer/render_system/d3d11/sampler_state.h"

#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {

namespace {
D3D11_FILTER TranslateSamplerStateCompFilter(const SamplerState::Options& state) {
  DCHECK_NE(state.compare_func, CompareFunc::kNever);
  if (state.comp_mag_filter == FilterMode::kPoint
      && state.comp_min_filter == FilterMode::kPoint
      && state.comp_mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
  } else if (state.comp_mag_filter == FilterMode::kLinear
             && state.comp_min_filter == FilterMode::kLinear
             && state.comp_mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  } else if (state.comp_mag_filter == FilterMode::kPoint
             && state.comp_min_filter == FilterMode::kLinear
             && state.comp_mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
  } else if (state.comp_mag_filter == FilterMode::kPoint
             && state.comp_min_filter == FilterMode::kLinear
             && state.comp_mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
  } else if (state.comp_mag_filter == FilterMode::kLinear
             && state.comp_min_filter == FilterMode::kPoint
             && state.comp_mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
  } else if (state.comp_mag_filter == FilterMode::kLinear
             && state.comp_min_filter == FilterMode::kPoint
             && state.comp_mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
  } else if (state.comp_mag_filter == FilterMode::kLinear
             && state.comp_min_filter == FilterMode::kLinear
             && state.comp_mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
  } else {
    NOTREACHED();
    return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  }
}

D3D11_FILTER TranslateSamplerStateFilter(const SamplerState::Options& state) {
  if (state.mag_filter == FilterMode::kPoint
      && state.min_filter == FilterMode::kPoint
      && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_MAG_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kPoint
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kPoint
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kPoint
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kPoint
             && state.mip_filter == FilterMode::kLinear) {
    return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
  } else if (state.mag_filter == FilterMode::kLinear
             && state.min_filter == FilterMode::kLinear
             && state.mip_filter == FilterMode::kPoint) {
    return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
  } else {
    NOTREACHED();
    return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  }
}
}  // namespace


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
  if (options().compare_func == CompareFunc::kNever) {
    sampler_desc.Filter = TranslateSamplerStateFilter(options());  
  } else {
    sampler_desc.Filter = TranslateSamplerStateCompFilter(options());  
  }
  sampler_desc.ComparisonFunc = TranslateCompareFunc(options().compare_func);
  
  sampler_desc.AddressU = TranslateTexWrapMode(options().wrap_u);
  sampler_desc.AddressV = TranslateTexWrapMode(options().wrap_v);
  sampler_desc.AddressW = TranslateTexWrapMode(options().wrap_w);
  
  sampler_desc.MaxAnisotropy = options().max_anisotropy;
  sampler_desc.MipLODBias = 0.0f;
  sampler_desc.MinLOD = 0;
  sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
  sampler_desc.BorderColor[0] = options().border_color.x;
  sampler_desc.BorderColor[1] = options().border_color.y;
  sampler_desc.BorderColor[2] = options().border_color.z;
  sampler_desc.BorderColor[3] = options().border_color.w;
  HRESULT hr = d3d_device->CreateSamplerState(&sampler_desc, &sampler_state_);
  HRESULT_HANDLE(hr, ERROR, "CreateSamplerState failed ");

  D3D11_SAMPLER_DESC sampler_desc2;
  sampler_state_->GetDesc(&sampler_desc2);
  return true;
}
}  // namespace d3d11
}  // namespace azer
