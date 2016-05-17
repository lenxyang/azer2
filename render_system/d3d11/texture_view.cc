#include "azer/render_system/d3d11/texture_view.h"

#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
D3DTexView::D3DTexView(const Options& options, Texture* tex)
    : TextureView(options, tex) {
}

D3DTexView::~D3DTexView() {
}

void D3DTexView::GenerateMips(int32 level) {
  CHECK(false);
}

// class D3DResTextureView
D3DResTextureView::D3DResTextureView(const Options& options, Texture* tex) 
    : D3DTexView(options, view),
      res_view_(NULL) {
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

bool D3DResTextureView::Init() {
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
      if (texture()->options().sample_desc.count > 1) {
        desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
      } else {
        desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      }
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

D3DUAResTextureView::D3DUAResTextureView(const Options& options, Texture* tex) 
    : D3DTexRes(options, tex),
      uav_view_(NULL) {
}

D3DUAResTextureView::~D3DUAResTextureView() {
  SAFE_RELEASE(uav_view_);
}

bool D3DUAResTextureView::Init() {
  ID3D11Device* d3d_device = render_system_->GetDevice();
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  desc.Format = tex_desc_.Format;
  desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipSlice = 0;
  HRESULT hr = d3d_device->CreateUnorderedAccessView(texres_, &desc, &uav_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateUnorderedAccessView failed ");
}
}  // namespace azer
}  // namespace d3d11
