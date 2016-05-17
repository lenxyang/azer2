#include "azer/render_system/d3d11/texture_view.h"

#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
D3DTexView::D3DTexView() {
}

D3DTexView::~D3DTexView() {
}

void D3DTexView::GenerateMips(int32 level) {
  CHECK(false);
}

// class D3DResTextureView
D3DResTextureView::D3DResTextureView() 
    : res_view_(NULL) {
}

D3DResTextureView::~D3DResTextureView() {
  SAFE_RELEASE(res_view_);
}

void D3DResTextureView::GenerateMips(int32 level) {
  // attention:
  // resource must be specified bind with Resource and RenderTarget
  // and misc flags must be with D3D11_RESOURCE_MISC_GENERATE_MIPS
  DCHECK(res_view_ != NULL);
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->GenerateMips(res_view_);
}

bool D3DResTextureView::InitRes() {
  ID3D11Device* d3d_device = render_system_->GetDevice();
  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_SRV_DIMENSION_TEXTURE2D,
    0,
    0,
  };

  switch (options().type) {
    case kTexCubemap: 
      view_desc.Format = tex_desc_.Format;
      view_desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
      view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      view_desc.TextureCube.MipLevels = tex_desc_.MipLevels;
      view_desc.TextureCube.MostDetailedMip = 0;
      break;
    case kTex2DArray:
      view_desc.Format = tex_desc_.Format;
      view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      view_desc.Texture2DArray.MipLevels = tex_desc_.MipLevels;
      view_desc.Texture2DArray.MostDetailedMip = 0;
      view_desc.Texture2DArray.FirstArraySlice = 0;
      view_desc.Texture2DArray.ArraySize = diminison_;
      break;
  }

  HRESULT hr = d3d_device->CreateShaderResourceView(texres_, &view_desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateResourceView failed for texture");
  return true;
}

D3DUAResTextureView::D3DUAResTextureView() 
    : uav_view_(NULL) {
}

D3DUAResTextureView::~D3DUAResTextureView() {
  SAFE_RELEASE(uav_view_);
}

}  // namespace azer
}  // namespace d3d11
