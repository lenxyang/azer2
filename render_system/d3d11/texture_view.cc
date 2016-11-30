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
  ID3D11Device* d3d_device = rs->GetDevice();
  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_SRV_DIMENSION_TEXTURE2D,
    0,
    0,
  };


  D3DTexture2D* tex = ((D3DTexture2D*)texture());
  const D3D11_TEXTURE2D_DESC& texdesc = tex->desc();
  TexType textype = (options().type == TexType::kUnknown) ?
	  tex->options().type : options().type;
  view_desc.Format = texdesc.Format;
  if (options().format != TexFormat::kUndefined) {
    view_desc.Format = TranslateTexFormat(options().format);
  }
  bool msaa = (texture()->options().sample_desc.count > 1);
  switch (textype) {
    case TexType::k2D:
      view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      view_desc.Texture2D.MipLevels = texdesc.MipLevels;
      view_desc.Texture2D.MostDetailedMip = 0;
      break;
    case TexType::kCubemap: 
      view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      view_desc.TextureCube.MipLevels = texdesc.MipLevels;
      view_desc.TextureCube.MostDetailedMip = 0;
      break;
    case TexType::k2DArray:
      if (msaa) {
        view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
      } else {
        view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
      }
      view_desc.Texture2DArray.MipLevels = texdesc.MipLevels;
      view_desc.Texture2DArray.MostDetailedMip = 0;
      view_desc.Texture2DArray.FirstArraySlice = 0;
      view_desc.Texture2DArray.ArraySize = 1; // tex->options().diminison;
      break;
    default:
      CHECK(false);
  }

  HRESULT hr = d3d_device->CreateShaderResourceView(
      tex->GetResource(), &view_desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateResourceView failed for texture");
  return true;
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
      tex->GetResource(), &desc, &uav_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateUnorderedAccessView failed ");
  return true;
}

void D3DTextureUAResView::GenerateMips(int32_t level) {
  CHECK(false) << "unsupport";
}
}  // namespace azer
}  // namespace d3d11
