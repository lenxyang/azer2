#include "azer/render_system/d3d11/texture_view.h"

#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
// class D3DResTextureView
D3DResTextureView::D3DResTextureView(const Options& options, Texture* tex) 
    : TextureView(options, tex),
      res_view_(NULL) {
  CHECK(tex != NULL) << "Texture cannot be NULL";
}

D3DResTextureView::~D3DResTextureView() {
  SAFE_RELEASE(res_view_);
}

void D3DResTextureView::GenerateMips(int32_t level) {
  // attention:
  // resource must be specified bind with Resource and RenderTarget
  // and misc flags must be with D3D11_RESOURCE_MISC_GENERATE_MIPS
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  DCHECK(res_view_ != NULL);
  ID3D11DeviceContext* d3d_context = rs->GetContext();
  d3d_context->GenerateMips(res_view_);
}

bool D3DResTextureView::Init(D3DRenderSystem* rs) {
  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_SRV_DIMENSION_TEXTURE2D,
    0,
    0,
  };
  if (options().format != TexFormat::kUndefined) {
    view_desc.Format = TranslateTexFormat(options().format);
  }
  ID3D11Device* d3d_device = rs->GetDevice();
  TexType textype = (options().type == TexType::kUnknown) ?
      texture()->type() : options().type;
  switch (textype) {
    case TexType::k2D:
      InitFor2D(&view_desc);
	  break;
    case TexType::kCubemap:
      InitForCubeMap(&view_desc);
	  break;
    case TexType::k2DArray:
      InitFor2DArray(&view_desc);
	  break;
    case TexType::k3D:
      InitFor3D(&view_desc);
	  break;
    default:
      return false;
  }

  HRESULT hr = d3d_device->CreateShaderResourceView(
      (ID3D11Resource*)texture()->native_handle(), &view_desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateResourceView failed for texture");
  return true;
}

void D3DResTextureView::InitFor2D(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  D3DTexture2D* tex = ((D3DTexture2D*)texture());
  const D3D11_TEXTURE2D_DESC& texdesc = tex->desc();
  desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  desc->Texture2D.MipLevels = texdesc.MipLevels;
  desc->Texture2D.MostDetailedMip = 0;
}

void D3DResTextureView::InitFor2DArray(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  bool msaa = (texture()->options().sample_desc.count > 1);
  D3DTexture2D* tex = ((D3DTexture2D*)texture());
  const D3D11_TEXTURE2D_DESC& texdesc = tex->desc();
  if (msaa) {
    desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
  } else {
    desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
  }
  desc->Texture2DArray.MipLevels = texdesc.MipLevels;
  desc->Texture2DArray.MostDetailedMip = 0;
  desc->Texture2DArray.FirstArraySlice = 0;
  desc->Texture2DArray.ArraySize = 1; // tex->options().diminison;
}

void D3DResTextureView::InitForCubeMap(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  D3DTexture2D* tex = ((D3DTexture2D*)texture());
  const D3D11_TEXTURE2D_DESC& texdesc = tex->desc();
  desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
  desc->TextureCube.MipLevels = texdesc.MipLevels;
  desc->TextureCube.MostDetailedMip = 0;
}

void D3DResTextureView::InitFor3D(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  D3DTexture3D* tex = ((D3DTexture3D*)texture());
  const D3D11_TEXTURE3D_DESC& texdesc = tex->desc();
  desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
  desc->Texture3D.MipLevels = texdesc.MipLevels;
  desc->Texture3D.MostDetailedMip = 0;
}

D3DTextureUAResView::D3DTextureUAResView(const Options& options, Texture* tex) 
    : TextureUAView(options, tex),
      uav_view_(NULL) {
}

D3DTextureUAResView::~D3DTextureUAResView() {
  SAFE_RELEASE(uav_view_);
}

bool D3DTextureUAResView::Init(D3DRenderSystem* rs) {
  D3DTexture2D* tex = (D3DTexture2D*)texture();
  ID3D11Device* d3d_device = rs->GetDevice();
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  desc.Format = tex->desc().Format;
  desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipSlice = 0;

  HRESULT hr = d3d_device->CreateUnorderedAccessView(
      (ID3D11Resource*)tex->native_handle(), &desc, &uav_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateUnorderedAccessView failed ");
  return true;
}

void D3DTextureUAResView::GenerateMips(int32_t level) {
  CHECK(false) << "unsupport";
}
}  // namespace azer
}  // namespace d3d11
