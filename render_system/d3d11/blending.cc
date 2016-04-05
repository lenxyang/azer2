#include "azer/render_system/d3d11/blending.h"

#include <d3d11.h>
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {

D3DBlending::D3DBlending(const BlendDesc& desc, D3DRenderSystem* rs)
    : Blending(desc)
    , blending_state_(NULL)
    , render_system_(rs) {
}

D3DBlending::~D3DBlending() {
  SAFE_RELEASE(blending_state_);
}

bool D3DBlending::Init() {
  D3D11_BLEND_DESC blend_desc;
  ZeroMemory(&blend_desc, sizeof(blend_desc));
  int32 count = std::min(kMaxRenderTargetDesc,
                         (int)arraysize(blend_desc.RenderTarget));
  for (int32 i = 0; i < count; ++i) {
    D3D11_RENDER_TARGET_BLEND_DESC& rtbd = blend_desc.RenderTarget[i];
    ZeroMemory(&rtbd, sizeof(rtbd));
    rtbd.BlendEnable = desc_.desc[i].enable;
    rtbd.SrcBlend = TranslateBlending(desc_.desc[i].src);
    rtbd.DestBlend = TranslateBlending(desc_.desc[i].dest);
    rtbd.BlendOp = TranslateBlendingOper(desc_.desc[i].oper);
    rtbd.SrcBlendAlpha = TranslateBlending(desc_.desc[i].src_alpha);
    rtbd.DestBlendAlpha = TranslateBlending(desc_.desc[i].dest_alpha);
    rtbd.BlendOpAlpha = TranslateBlendingOper(desc_.desc[i].alpha_oper);
    rtbd.RenderTargetWriteMask = TranslateWriteMask(desc_.desc[i].mask);
  }

  blend_desc.AlphaToCoverageEnable = desc_.alpha_to_converage;
  blend_desc.IndependentBlendEnable = desc_.independent_blending;
  
  ID3D11Device* d3d_device = render_system_->GetDevice();
  DCHECK(NULL != d3d_device);
  ID3D11BlendState* state = NULL;
  HRESULT hr = d3d_device->CreateBlendState(&blend_desc, &blending_state_);
  HRESULT_HANDLE(hr, ERROR, "CreateBlendState failed ");
  
  return true;
}

bool D3DBlending::OnDescChanged() {
  if (blending_state_) {
  }
  return true;
}
}  // namespace d3d11
}  // namespace azer
