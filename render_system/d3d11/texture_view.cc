#include "azer/render_system/d3d11/texture_view.h"

#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/render_system.h"

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
    : D3DTexView(options, tex),
      res_view_(NULL) {
}

D3DResTextureView::~D3DResTextureView() {
  SAFE_RELEASE(res_view_);
}

void D3DResTextureView::GenerateMips(int32 level) {
  // attention:
  // resource must be specified bind with Resource and RenderTarget
  // and misc flags must be with D3D11_RESOURCE_MISC_GENERATE_MIPS
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(res_view_ != NULL);
  ID3D11Device* d3d_device = rs->GetDevice();
  ID3D11DeviceContext* d3d_context = rs->GetContext();
  d3d_context->GenerateMips(res_view_);
}

bool D3DResTextureView::Init() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11Device* d3d_device = rs->GetDevice();
  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_SRV_DIMENSION_TEXTURE2D,
    0,
    0,
  };

  const D3D11_TEXTURE2D_DESC& texdesc = ((D3DTexture*)texture())->desc();
  switch (options().type) {
    case kTexCubemap: 
      view_desc.Format = texdesc.Format;
      view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      view_desc.TextureCube.MipLevels = texdesc.MipLevels;
      view_desc.TextureCube.MostDetailedMip = 0;
      break;
    case kTex2DArray:
      view_desc.Format = texdesc.Format;
      if (texture()->options().sample_desc.count > 1) {
        view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
      } else {
        view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      }
      view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      view_desc.Texture2DArray.MipLevels = texdesc.MipLevels;
      view_desc.Texture2DArray.MostDetailedMip = 0;
      view_desc.Texture2DArray.FirstArraySlice = 0;
      view_desc.Texture2DArray.ArraySize = texture()->diminison();
      break;
  }

  D3DTexture* tex = (D3DTexture*)texture();
  HRESULT hr = d3d_device->CreateShaderResourceView(
      tex->GetResource(), &view_desc, &res_view_);
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
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11Device* d3d_device = rs->GetDevice();
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  desc.Format = tex_desc_.Format;
  desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipSlice = 0;

  D3DTexture* tex = (D3DTexture*)texture();
  HRESULT hr = d3d_device->CreateUnorderedAccessView(
      tex->GetResource(), &desc, &uav_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateUnorderedAccessView failed ");
}
}  // namespace azer
}  // namespace d3d11
