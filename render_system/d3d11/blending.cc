#include "azer/render_system/d3d11/blending.h"

#include <d3d11.h>
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {

D3DBlending::D3DBlending(const Desc& desc, D3DRenderSystem* rs)
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

  D3D11_RENDER_TARGET_BLEND_DESC rtbd;
  ZeroMemory(&rtbd, sizeof(rtbd));

  rtbd.BlendEnable = true;
  rtbd.SrcBlend = TranslateBlending(desc_.src);
  rtbd.DestBlend = TranslateBlending(desc_.dest);
  rtbd.BlendOp = TranslateBlendingOper(desc_.oper);
  rtbd.SrcBlendAlpha = TranslateBlending(desc_.src_alpha);
  rtbd.DestBlendAlpha = TranslateBlending(desc_.dest_alpha);
  rtbd.BlendOpAlpha = TranslateBlendingOper(desc_.alpha_oper);
  rtbd.RenderTargetWriteMask = TranslateWriteMask(desc_.mask);

  blend_desc.AlphaToCoverageEnable = enable_alpha_to_converage_;
  blend_desc.IndependentBlendEnable = enable_independent_blending_;
  blend_desc.RenderTarget[0] = rtbd;
  
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
