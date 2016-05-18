#include "azer/render_system/d3d11/texture_view.h"

#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
D3DTextureView::D3DTextureView(const Options& options, Texture* tex)
    : TextureView(options, tex) {
  DCHECK(CheckTexFormatCapability());
  D3DTexture* t = (D3DTexture*)tex;
  t->GetResource()->AddRef();
}

D3DTextureView::~D3DTextureView() {
  ID3D11Resource* res = ((D3DTexture*)texture())->GetResource();
  SAFE_RELEASE(res);
}

void D3DTextureView::GenerateMips(int32 level) {
  CHECK(false);
}

bool D3DTextureView::CheckTexFormatCapability() {
  if (! (texture()->options().target & kBindTargetShaderResource)) {
    return false;
  }
  return true;
}

// class D3DResTextureView
D3DResTextureView::D3DResTextureView(const Options& options, Texture* tex) 
    : D3DTextureView(options, tex),
      res_view_(NULL) {
}

D3DResTextureView::~D3DResTextureView() {
  D3DTexture* tex = ((D3DTexture*)texture());
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


  D3DTexture* tex = ((D3DTexture*)texture());
  const D3D11_TEXTURE2D_DESC& texdesc = tex->desc();
  TexType textype = (options().type == kUnkonwnTexType) ?
	  tex->options().type : options().type;
  switch (textype) {
    case kTex2D:
	    view_desc.Format = texdesc.Format;
	    view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      view_desc.Texture2D.MipLevels = texdesc.MipLevels;
      view_desc.Texture2D.MostDetailedMip = 0;
	    break;
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
      view_desc.Texture2DArray.ArraySize = tex->diminison();
      break;
  }

  HRESULT hr = d3d_device->CreateShaderResourceView(
      tex->GetResource(), &view_desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateResourceView failed for texture");
  return true;
}

D3DUAResTextureView::D3DUAResTextureView(const Options& options, Texture* tex) 
    : D3DTextureView(options, tex),
      uav_view_(NULL) {
}

D3DUAResTextureView::~D3DUAResTextureView() {
  SAFE_RELEASE(uav_view_);
}

bool D3DUAResTextureView::Init() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  D3DTexture* tex = (D3DTexture*)texture();
  ID3D11Device* d3d_device = rs->GetDevice();
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  desc.Format = tex->desc().Format;
  desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipSlice = 0;

  
  HRESULT hr = d3d_device->CreateUnorderedAccessView(
      tex->GetResource(), &desc, &uav_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateUnorderedAccessView failed ");
  return true;
}
}  // namespace azer
}  // namespace d3d11
