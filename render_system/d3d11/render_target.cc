#include "azer/render_system/d3d11/render_target.h"

#include <string>

#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/swap_chain.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

D3DRenderTarget::D3DRenderTarget(const Options& opt, bool surface_target, 
                                 D3DRenderSystem* render_system)
    : RenderTarget(opt, surface_target),
      target_(NULL),
      texres_(NULL),
      render_system_(render_system) {
}
  
D3DRenderTarget::~D3DRenderTarget() {
  SAFE_RELEASE(target_);
  SAFE_RELEASE(texres_);
}

void D3DRenderTarget::Clear(const azer::Vector4& color) {
  DCHECK(NULL != target_);
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->ClearRenderTargetView(target_, (float*)&color);
}

bool D3DRenderTarget::Init(D3DTexture2DArray* tex, const Texture2DArray::Slice& slice) {
  DCHECK(!default_render_target_);
  DCHECK(target_ == NULL);
  DCHECK(texres_ == NULL);

  ID3D11Device* d3d_device = render_system_->GetDevice();
  HRESULT hr = S_OK;
  DCHECK(texture_.get() == NULL);
  texture_ = tex;

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  desc.Format = TranslateTexFormat(tex->options().format);
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
  desc.Texture2DArray.FirstArraySlice = slice.first_slice;
  desc.Texture2DArray.ArraySize = slice.array_size;
  desc.Texture2DArray.MipSlice = slice.mip_slice;


  texres_ = (ID3D11Resource*)tex->native_handle();
  texres_->AddRef();
  DCHECK(TranslateBindTarget(tex->options().target) & D3D11_BIND_RENDER_TARGET);
  hr = d3d_device->CreateRenderTargetView(texres_, &desc, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");

  return true;
}

bool D3DRenderTarget::Init(D3DTexture2D* tex) {
  DCHECK(!default_render_target_);
  DCHECK(target_ == NULL);
  DCHECK(texres_ == NULL);

  ID3D11Device* d3d_device = render_system_->GetDevice();
  HRESULT hr = S_OK;
  DCHECK(texture_.get() == NULL);
  texture_ = tex;

  texres_ = (ID3D11Resource*)tex->native_handle();
  texres_->AddRef();
  DCHECK(TranslateBindTarget(tex->options().target) & D3D11_BIND_RENDER_TARGET);
  hr = d3d_device->CreateRenderTargetView(texres_, NULL, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateRenderTargetView failed ");

  return true;
}

void D3DRenderTarget::SetName(const std::string& name) {
  DCHECK(target_);
  target_->SetPrivateData(WKPDID_D3DDebugObjectName,
                          (UINT)name.length(), name.c_str());
}

NativeGpuResourceViewHandle D3DRenderTarget::native_handle() {
  CHECK(false);
  return (NativeGpuResourceViewHandle)target_;
}
}  // namespace d3d11
}  // namespace azer
