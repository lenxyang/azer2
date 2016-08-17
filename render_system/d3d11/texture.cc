// reference: 
// 1. Introduction To Textures in Direct3D 11[http://msdn.microsoft.com/en-us/library/windows/desktop/ff476906(v=vs.85).aspx]
// 2. Creating Texture Resources (Direct3D 10) [http://msdn.microsoft.com/en-us/library/windows/desktop/bb205131(v=vs.85).aspx]
// 3. Coordinate Systems (Direct3D 10) [http://msdn.microsoft.com/en-us/library/windows/desktop/cc308049(v=vs.85).aspx]
// 4. How to: Initialize a Texture From a File [http://msdn.microsoft.com/en-us/library/windows/desktop/ff476904(v=vs.85).aspx]

#include "azer/render_system/d3d11/texture.h"

#include "base/logging.h"
#include "base/strings/string16.h"
#include "azer/base/image.h"
#include "azer/render_system/d3d11/common.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {

D3DTexture::D3DTexture(const Options& opt, D3DRenderSystem* rs)
    : Texture(opt),
      texres_(NULL),
      render_system_(rs),
      diminison_(0) {
}

D3DTexture::~D3DTexture() {
  SAFE_RELEASE(texres_);
}

NativeGpuResourceHandle D3DTexture::native_handle() {
  return (NativeGpuResourceHandle)texres_;
}

void D3DTexture::InitTexDesc() {
  ZeroMemory(&tex_desc_, sizeof(D3D11_TEXTURE2D_DESC));
  tex_desc_.Width     = options_.size.width();
  tex_desc_.Height    = options_.size.height();
  tex_desc_.MipLevels = options_.mipmap_level;
  tex_desc_.ArraySize = options_.diminison;
  tex_desc_.Format    = TranslateTexFormat(options_.format);
  tex_desc_.SampleDesc.Count   = options_.sample_desc.count;
  tex_desc_.SampleDesc.Quality = options_.sample_desc.quality;
  tex_desc_.Usage          = TranslateUsage(options_.usage);
  tex_desc_.BindFlags      = TranslateBindTarget(options_.target);
  tex_desc_.CPUAccessFlags = TranslateCPUAccess(options_.cpu_access);
  tex_desc_.MiscFlags      = options_.genmipmap ?  
      D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
}

bool D3DTexture::Init(const D3D11_SUBRESOURCE_DATA* data) {
  HRESULT hr = S_OK;
  DCHECK(NULL == texres_);
  ID3D11Device* d3d_device = render_system_->GetDevice();
  InitTexDesc();

  ID3D11Texture2D* tex = NULL;
  hr = d3d_device->CreateTexture2D(&tex_desc_, data, &tex);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");

  texres_ = tex;
  return true;
}

void D3DTexture::SetName(const std::string& name) {
  DCHECK(texres_);
  texres_->SetPrivateData(WKPDID_D3DDebugObjectName,
                          (UINT)name.length(), name.c_str());
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
GpuResLockDataPtr D3DTexture::map(MapType flags) {
  map_helper_ = new GpuResLockHelper(resource_options(), texres_);
  return map_helper_->map(flags);
}

void D3DTexture::unmap() {
  CHECK(false) << "always should not called";
  CHECK(map_helper_.get());
  map_helper_->unmap();
  map_helper_ = NULL;
}

bool D3DTexture::CopyTo(GpuResource* texres) {
  DCHECK_EQ(texres->resource_type(), GpuResType::kTexture);
  D3DTexture* tex = (D3DTexture*)texres;
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
  } else if (options().sample_desc.count > 1 
             && tex->options().sample_desc.count == 1) {
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
  DCHECK_LT(image->level_count(), static_cast<int32_t>(arraysize(subres)));
  for (int32_t i = 0; i < image->level_count(); ++i, ++count) {
    const ImageLevelData* data = image->GetLevelData(i);
    subres[i].pSysMem = data->dim_data(0);
    subres[i].SysMemPitch = data->row_bytes();
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres);
}


// class D3D11TextureCubeMap
D3DTextureCubeMap::D3DTextureCubeMap(const Options& opt, 
                                     D3DRenderSystem* rs)
    : D3DTexture(opt, rs) {
  CHECK_EQ(opt.diminison, 6);
}

bool D3DTextureCubeMap::InitFromImage(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  D3D11_SUBRESOURCE_DATA subres[6] = { 0 };
  for (int i = 0; i < 6; ++i) {
    const ImageLevelData* data = image->GetLevelData(i);

    subres[i].pSysMem = data->dim_data(0);
    subres[i].SysMemPitch = data->row_bytes();
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres);
}

void D3DTextureCubeMap::InitTexDesc() {
  D3DTexture::InitTexDesc();
  tex_desc_.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
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
  CHECK_EQ(image->depth(), options().diminison);
  return Init(subres);
}

}  // namespace d3d11
}  // namespace azer
