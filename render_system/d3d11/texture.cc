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

#define IMPLEMENT_TEXMEMBER(class_name)                                 \
void class_name::SetName(const std::string& name) {                     \
  DCHECK(texres_);                                                      \
  texres_->SetPrivateData(WKPDID_D3DDebugObjectName,                    \
                          (UINT)name.length(), name.c_str());           \
}                                                                       \
GpuResLockDataPtr class_name::map(MapType flags) {                      \
  map_helper_ = new GpuResLockHelper(resource_options(), texres_);      \
  return map_helper_->map(flags);                                       \
}                                                                       \
void class_name::unmap() {                                              \
  CHECK(false) << "always should not called";                           \
  CHECK(map_helper_.get());                                             \
  map_helper_->unmap();                                                 \
  map_helper_ = NULL;                                                   \
}                                                                       \
void class_name::Attach(ID3D11Texture2D* tex) {                         \
  DCHECK(texres_ == NULL);                                              \
  DCHECK(tex != NULL);                                                  \
  texres_ = tex;                                                        \
  texres_->AddRef();                                                    \
}                                                                       \
void class_name::Detach() {                                             \
  SAFE_RELEASE(texres_);                                                \
  texres_ = NULL;                                                       \
}                                                                       \
bool class_name::CopyTo(GpuResource* texres) {                          \
  return TextureCopyTo(this, texres);                                   \
}

namespace azer {
namespace d3d11 {

namespace {
bool TextureCopyTo(Texture* src, GpuResource* destres);
void InitTexture2DDescFromOptions(const Texture::Options& options,
                                  D3D11_TEXTURE2D_DESC *desc);
}

IMPLEMENT_TEXMEMBER(D3DTexture2D);
IMPLEMENT_TEXMEMBER(D3DTexture2DArray);
IMPLEMENT_TEXMEMBER(D3DTextureCubeMap);

// class D3DTexture2D
D3DTexture2D::D3DTexture2D(const Options& opt, D3DRenderSystem* rs)
    : Texture2D(opt),
      texres_(NULL),
      render_system_(rs) {
}

D3DTexture2D::~D3DTexture2D() {
  SAFE_RELEASE(texres_);
}

bool D3DTexture2D::InitFromData(const D3D11_SUBRESOURCE_DATA* data) {
  DCHECK(NULL == native_handle());
  HRESULT hr = S_OK;
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11Texture2D* tex = NULL;
  InitTexture2DDescFromOptions(options(), &texdesc_);
  hr = d3d_device->CreateTexture2D(&texdesc_, data, &tex);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");

  texres_ = tex;
  return true;
}

bool D3DTexture2D::Init(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  if (image) {
    int count = 0;
    D3D11_SUBRESOURCE_DATA subres[128] = { 0 };
    DCHECK_LT(image->level_count(), static_cast<int>(arraysize(subres)));
    for (int i = 0; i < image->level_count(); ++i, ++count) {
      const ImageLevelData* data = image->GetLevelData(i);
      subres[i].pSysMem = data->dim_data(0);
      subres[i].SysMemPitch = data->row_bytes();
      subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
    }
    return InitFromData(subres);
  } else {
    return InitFromData(NULL);
  }
}

// class D3D11TextureCubeMap
D3DTextureCubeMap::D3DTextureCubeMap(const Options& opt, D3DRenderSystem* rs)
    : TextureCubemap(opt),
	texres_(NULL) {
  CHECK_EQ(opt.size.width, opt.size.height);
  CHECK_EQ(opt.size.depth, 1);
}

bool D3DTextureCubeMap::Init(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  if (image) {
    D3D11_SUBRESOURCE_DATA subres[6] = { 0 };
    for (int i = 0; i < 6; ++i) {
      const ImageLevelData* data = image->GetLevelData(i);

      subres[i].pSysMem = data->dim_data(0);
      subres[i].SysMemPitch = data->row_bytes();
      subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
    }

    return InitFromData(subres);
  } else {
    return InitFromData(NULL);
  }
}

bool D3DTextureCubeMap::InitFromData(const D3D11_SUBRESOURCE_DATA* data) {
  DCHECK(NULL == native_handle());
  HRESULT hr = S_OK;
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11Device* d3d_device = rs->GetDevice();
  InitTexture2DDescFromOptions(options(), &texdesc_);
  texdesc_.ArraySize = 6;
  texdesc_.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
  
  hr = d3d_device->CreateTexture2D(&texdesc_, data, &texres_);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");
  return true;
}

// class D3D11TextureCubeMap
D3DTexture2DArray::D3DTexture2DArray(const Options& opt, D3DRenderSystem* rs)
    : Texture2DArray(opt, opt.size.depth),
	texres_(NULL) {
}

bool D3DTexture2DArray::Init(const ImageData* image) {
  if (image) {
    // [reference] MSDN: How to: Initialize a Texture Programmatically
    D3D11_SUBRESOURCE_DATA subres[kTex2DArrayMaxDepth] = { 0 };
    CHECK_LT(image->depth(), kTex2DArrayMaxDepth);
    for (int i = 0; i < image->depth(); ++i) {
      const ImageLevelData* data = image->GetLevelData(0);
      subres[i].pSysMem = data->dim_data(i);
      subres[i].SysMemPitch = data->dim_data_size();
      subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
    }
    CHECK_EQ(image->depth(), this->diminison());
    return InitFromData(subres);
  } else {
    return InitFromData(NULL);
  }
}

bool D3DTexture2DArray::InitFromData(const D3D11_SUBRESOURCE_DATA* data) {
  DCHECK(NULL == native_handle());
  HRESULT hr = S_OK;
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11Device* d3d_device = rs->GetDevice();
  InitTexture2DDescFromOptions(options(), &texdesc_);
  texdesc_.ArraySize = this->size().depth;
  
  hr = d3d_device->CreateTexture2D(&texdesc_, data, &texres_);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");
  return true;
}

namespace {
bool TextureCopyTo(Texture* src, GpuResource* destres) {
  DCHECK_EQ(destres->resource_type(), GpuResType::kTexture);
  Texture* dest = (Texture*)destres;
  if (src->type() != dest->type()) {
    DLOG(INFO) << "cannot Copy Texture to diffuse type texture.";
    return false;
  }

  if (dest->size() != src->size()) {
    DLOG(INFO) << "cannot Copy Texture to the one with diffuse dimension.";
    return false;
  }

  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11DeviceContext* d3d_context = rs->GetContext();
  ID3D11Resource* src_native = (ID3D11Resource*)src->native_handle();
  ID3D11Resource* dest_native = (ID3D11Resource*)dest->native_handle();
  if (src->options().sample_desc.count == dest->options().sample_desc.count) {
    d3d_context->CopyResource(src_native, dest_native);
  } else if (src->options().sample_desc.count > 1 
             && dest->options().sample_desc.count == 1) {
    d3d_context->ResolveSubresource(
        src_native, 0, dest_native, 0, TranslateTexFormat(dest->options().format));
  } else {
    CHECK(false) << "not support";
  }
  return true;
}

void InitTexture2DDescFromOptions(const Texture::Options& options,
                                  D3D11_TEXTURE2D_DESC *desc) {
  ZeroMemory(desc, sizeof(D3D11_TEXTURE2D_DESC));
  desc->Width     = options.size.width;
  desc->Height    = options.size.height;
  desc->MipLevels = options.mipmap_level;
  desc->ArraySize = 1;
  desc->Format    = TranslateTexFormat(options.format);
  desc->SampleDesc.Count   = options.sample_desc.count;
  desc->SampleDesc.Quality = options.sample_desc.quality;
  desc->Usage          = TranslateUsage(options.usage);
  desc->BindFlags      = TranslateBindTarget(options.target);
  desc->CPUAccessFlags = TranslateCPUAccess(options.cpu_access);
  desc->MiscFlags      = options.genmipmap ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
}
}
}  // namespace d3d11
}  // namespace azer
