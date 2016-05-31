// reference: 
// 1. Introduction To Textures in Direct3D 11[http://msdn.microsoft.com/en-us/library/windows/desktop/ff476906(v=vs.85).aspx]
// 2. Creating Texture Resources (Direct3D 10) [http://msdn.microsoft.com/en-us/library/windows/desktop/bb205131(v=vs.85).aspx]
// 3. Coordinate Systems (Direct3D 10) [http://msdn.microsoft.com/en-us/library/windows/desktop/cc308049(v=vs.85).aspx]
// 4. How to: Initialize a Texture From a File [http://msdn.microsoft.com/en-us/library/windows/desktop/ff476904(v=vs.85).aspx]

#include "azer/render_system/d3d11/texture.h"

#include <d3d11.h>
#include <d3dx11.h>

#include "base/logging.h"
#include "base/strings/string16.h"
#include "azer/base/image.h"
#include "azer/render_system/d3d11/common.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

D3DTexture::D3DTexture(const Options& opt, D3DRenderSystem* rs)
    : Texture(opt),
      texres_(NULL),
      render_system_(rs),
      diminison_(0) {
#ifdef DEBUG
  mapped_ = false;
#endif
}

D3DTexture::~D3DTexture() {
  SAFE_RELEASE(texres_);
}

bool D3DTexture::Init(const D3D11_SUBRESOURCE_DATA* data, 
                      int arraysize, int mipmap) {
  HRESULT hr = S_OK;
  DCHECK(NULL == texres_);
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ZeroMemory(&tex_desc_, sizeof(D3D11_TEXTURE2D_DESC));
  tex_desc_.Width     = options_.size.width();
  tex_desc_.Height    = options_.size.height();
  tex_desc_.MipLevels = mipmap;
  tex_desc_.ArraySize = arraysize;
  tex_desc_.Format    = TranslateTexFormat(options_.format);
  tex_desc_.SampleDesc.Count   = options_.sample_desc.count;
  tex_desc_.SampleDesc.Quality = options_.sample_desc.quality;
  tex_desc_.Usage          = TranslateUsage(options_.usage);
  tex_desc_.BindFlags      = TranslateBindTarget(options_.target);
  tex_desc_.CPUAccessFlags = TranslateCPUAccess(options_.cpu_access);
  tex_desc_.MiscFlags      = options_.genmipmap ?  
      D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

  if (options().type == kTexCubemap) {
    tex_desc_.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
  }

  ID3D11Texture2D* tex = NULL;
  hr = d3d_device->CreateTexture2D(&tex_desc_, data, &tex);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");

  texres_ = tex;
  return true;
}

void D3DTexture::Attach(ID3D11Texture2D* tex) { 
  DCHECK(texres_ == NULL);
  DCHECK(tex != NULL);
  texres_ = tex;
  texres_->AddRef();
}

void D3DTexture::Detach() { 
  SAFE_RELEASE(texres_);
  texres_ = NULL; 
}

// reference: MSDN "How to: Use dynamic resources"
Texture::MapData D3DTexture::map(MapType maptype) {
  DCHECK(NULL != texres_);
  MapData mapdata;
  ZeroMemory(&mapdata, sizeof(mapdata));
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();

  D3D11_MAPPED_SUBRESOURCE mapped;
  ZeroMemory(&mapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
  HRESULT hr = d3d_context->Map(texres_, 0, TranslateMapType(maptype), 0, &mapped);
  if (FAILED(hr)) {
    LOG(ERROR) << "Map Texture failed.";
    return mapdata;
  }
  
  mapdata.pdata = (uint8*)mapped.pData;
  mapdata.row_pitch = mapped.RowPitch;
  mapdata.depth_pitch = mapped.DepthPitch;

#ifdef DEBUG
  DCHECK(!mapped_);
  mapped_ = true;
#endif
  return mapdata;
}

void D3DTexture::unmap() {
#ifdef DEBUG
  DCHECK(mapped_);
  mapped_ = false;
#endif
  DCHECK(NULL != texres_);
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->Unmap(texres_, 0);
}

bool D3DTexture::CopyTo(Texture* texture) {
  D3DTexture* tex = (D3DTexture*)texture;
  if (tex->options().type != options().type) {
    DLOG(INFO) << "cannot Copy Texture to diffuse type texture.";
    return false;
  }

  if (tex->options().size != options().size) {
    DLOG(INFO) << "cannot Copy Texture to the one with diffuse dimension.";
    return false;
  }

  if (options().sample_desc.count == tex->options().sample_desc.count) {
    ID3D11DeviceContext* d3d_context = render_system_->GetContext();
    d3d_context->CopyResource(tex->texres_, texres_);
  } else if (options().sample_desc.count > 1 && tex->options().sample_desc.count == 1) {
    ID3D11DeviceContext* d3d_context = render_system_->GetContext();
    d3d_context->ResolveSubresource(
        tex->texres_, 0, texres_, 0, TranslateTexFormat(tex->options().format));
  } else {
    CHECK(false) << "not support";
  }
  
  return true;
}

// class D3DTexture2D
bool D3DTexture2D::InitFromImage(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  int32_t count = 0;
  D3D11_SUBRESOURCE_DATA subres[128] = { 0 };
  DCHECK_LT(image->level_count(), static_cast<int32>(arraysize(subres)));
  for (int32_t i = 0; i < image->level_count(); ++i, ++count) {
    const ImageLevelData* data = image->GetLevelData(i);
    subres[i].pSysMem = data->dim_data(0);
    subres[i].SysMemPitch = data->row_bytes();
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres, image->depth(), count);
}



// class D3D11TextureCubeMap
D3DTextureCubeMap::D3DTextureCubeMap(const Options& opt, 
                                     D3DRenderSystem* rs)
    : D3DTexture(opt, rs) {
}

bool D3DTextureCubeMap::InitFromImage(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  D3D11_SUBRESOURCE_DATA subres[6];
  for (int i = 0; i < 6; ++i) {
    const ImageLevelData* data = image->GetLevelData(i);

    subres[i].pSysMem = data->dim_data(0);
    subres[i].SysMemPitch = data->dim_data_size();
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres, 6, 1);
}

// class D3D11TextureCubeMap
D3DTexture2DArray::D3DTexture2DArray(const Options& opt, D3DRenderSystem* rs)
    : D3DTexture(opt, rs) {
}

bool D3DTexture2DArray::InitFromImage(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  D3D11_SUBRESOURCE_DATA subres[kTex2DArrayMaxDepth] = { 0 };
  CHECK_LT(image->depth(), kTex2DArrayMaxDepth);
  diminison_ = image->depth();
  for (int i = 0; i < image->depth(); ++i) {
    const ImageLevelData* data = image->GetLevelData(0);
    subres[i].pSysMem = data->dim_data(i);
    subres[i].SysMemPitch = data->dim_data_size();
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres, image->depth(), 1);
}

}  // namespace d3d11
}  // namespace azer
