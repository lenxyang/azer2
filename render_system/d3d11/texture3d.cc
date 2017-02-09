#include "azer/render_system/d3d11/texture.cc"

#include "azer/render_system/d3d11/common.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {

namespace {
void InitTexture3DDescFromOptions(const Texture::Options& options,
                                  D3D11_TEXTURE3D_DESC *desc) {
  ZeroMemory(desc, sizeof(D3D11_TEXTURE3D_DESC));
  desc->BindFlags = D3D10_BIND_SHADER_RESOURCE;
  desc->CPUAccessFlags = 0;
  desc->Depth = options.size.depth;
  desc->Height = options.size.height;
  desc->Width = options.size.width;
  desc->Format = TranslateTexFormat(options.format);
  desc->Usage = TranslateUsage(options.usage);
  desc->MipLevels = 1;
  desc->MiscFlags = 0;
}
}

// class D3D11Texture3D
D3DTexture3D::D3DTexture3D(const Options& opt, D3DRenderSystem* rs)
    : Texture3D(opt),
      texres_(NULL),
      render_system_(rs) {
}

D3DTexture3D::~D3DTexture3D() {
  SAFE_RELEASE(texres_);
}

void D3DTexture3D::SetName(const std::string& name) {
  DCHECK(texres_);
  texres_->SetPrivateData(WKPDID_D3DDebugObjectName, 
                            (UINT)name.length(), name.c_str());
}

bool D3DTexture3D::Init(const ImageData* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  if (image) {
    D3D11_SUBRESOURCE_DATA subres[128] = { 0 };
    for (int i = 0; i < image->level_count(); ++i) {
      const ImageLevelData* data = image->GetLevelData(i);
      subres[i].pSysMem = data->dim_data(0);
      subres[i].SysMemPitch = data->row_bytes();
      subres[i].SysMemSlicePitch = data->row_bytes() * data->height();
    }
    return InitFromData(subres);
  } else {
    return InitFromData(NULL);
  }
}

bool D3DTexture3D::InitFromData(const D3D11_SUBRESOURCE_DATA* data) {
  HRESULT hr = S_OK;
  DCHECK(NULL == native_handle());
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11Texture3D* tex = NULL;
  InitTexture3DDescFromOptions(options(), &texdesc_);
  hr = d3d_device->CreateTexture3D(&texdesc_, data, &tex);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");

  texres_ = tex;
  return true;
}

GpuResLockDataPtr D3DTexture3D::map(MapType type) {
  CHECK(false);
  return NULL;
}
void D3DTexture3D::unmap() {
  CHECK(false);
}
}  // namespace d3d11
}  // namespace azer
