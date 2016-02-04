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

D3DTexture::D3DTexture(const Texture::Options& opt, D3DRenderSystem* rs)
    : Texture(opt)
    , texres_(NULL)
    , render_system_(rs)
    , sampler_state_(NULL)
    , res_view_(NULL),
      uav_view_(NULL) {
#ifdef DEBUG
  mapped_ = false;
#endif
}

D3DTexture::~D3DTexture() {
  SAFE_RELEASE(texres_);
  SAFE_RELEASE(res_view_);
  SAFE_RELEASE(sampler_state_);
  SAFE_RELEASE(uav_view_);
}

bool D3DTexture::Init(const D3D11_SUBRESOURCE_DATA* data, int num) {
  HRESULT hr;
  DCHECK(NULL == texres_);
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ZeroMemory(&tex_desc_, sizeof(D3D11_TEXTURE2D_DESC));
  tex_desc_.Width     = options_.size.width();
  tex_desc_.Height    = options_.size.height();
  tex_desc_.MipLevels = options_.sampler.mip_level;
  tex_desc_.ArraySize = num;
  tex_desc_.Format    = TranslateFormat(options_.format);
  tex_desc_.SampleDesc.Count   = options_.sampler.sample_level;
  tex_desc_.SampleDesc.Quality = options_.sampler.sample_quality;
  tex_desc_.Usage          = TranslateUsage(options_.usage);
  tex_desc_.BindFlags      = TranslateBindTarget(options_.target);
  tex_desc_.CPUAccessFlags = TranslateCPUAccess(options_.cpu_access);
  tex_desc_.MiscFlags      = options_.genmipmap ?  
      D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
  ModifyTextureDesc(&tex_desc_);

  ID3D11Texture2D* tex = NULL;
  hr = d3d_device->CreateTexture2D(&tex_desc_, data, &tex);
  HRESULT_HANDLE(hr, ERROR, "CreateTexture2D failed ");

  texres_ = tex;
  if (options_.target & kBindTargetShaderResource) {
    if (!InitResourceView()) 
      return false;
  }

  if (options_.target & kBindTargetUnorderedAccess) {
    return InitUnorderedAccessView();
  }
  return true;
}

void D3DTexture::SetPSSampler(int index, D3DRenderer* renderer) {
  DCHECK(NULL != res_view_);
  DCHECK_GE(index, 0);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->PSSetShaderResources(index, 1, &res_view_);
  if (sampler_state_) {
    d3d_context->PSSetSamplers(index, 1, &sampler_state_);
  } else {
    d3d_context->PSSetSamplers(index, 1, NULL);
  }
}

void D3DTexture::SetVSSampler(int index, D3DRenderer* renderer) {
  DCHECK(NULL != res_view_);
  DCHECK_GE(index, 0);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->VSSetShaderResources(index, 1, &res_view_);
  if (sampler_state_) {
    d3d_context->VSSetSamplers(index, 1, &sampler_state_);
  } else {
    d3d_context->VSSetSamplers(index, 1, NULL);
  }
}

void D3DTexture::SetHSSampler(int index, D3DRenderer* renderer) {
  DCHECK(NULL != res_view_);
  DCHECK_GE(index, 0);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->HSSetShaderResources(index, 1, &res_view_);
  if (sampler_state_) {
    d3d_context->HSSetSamplers(index, 1, &sampler_state_);
  } else {
    d3d_context->HSSetSamplers(index, 1, NULL);
  }
}

void D3DTexture::SetDSSampler(int index, D3DRenderer* renderer) {
  DCHECK(NULL != res_view_);
  DCHECK_GE(index, 0);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->DSSetShaderResources(index, 1, &res_view_);
  if (sampler_state_) {
    d3d_context->DSSetSamplers(index, 1, &sampler_state_);
  } else {
    d3d_context->DSSetSamplers(index, 1, NULL);
  }
}

void D3DTexture::SetGSSampler(int index, D3DRenderer* renderer) {
  DCHECK(NULL != res_view_);
  DCHECK_GE(index, 0);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->GSSetShaderResources(index, 1, &res_view_);
  if (sampler_state_) {
    d3d_context->GSSetSamplers(index, 1, &sampler_state_);
  } else {
    d3d_context->GSSetSamplers(index, 1, NULL);
  }
}

void D3DTexture::GenerateMips(int level) {
  // attention:
  // resource must be specified bind with Resource and RenderTarget
  // and misc flags must be with D3D11_RESOURCE_MISC_GENERATE_MIPS
  DCHECK(res_view_ != NULL);
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->GenerateMips(res_view_);
}

bool D3DTexture::InitResourceView() {
  ID3D11Device* d3d_device = render_system_->GetDevice();
  D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
  InitResourceDesc(&view_desc);
  HRESULT hr = d3d_device->CreateShaderResourceView(texres_, &view_desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateResourceView failed for texture");
  return SetSamplerState(options_.sampler);
}

bool D3DTexture::SetSamplerState(const SamplerState& sampler_state) {
  ID3D11Device* d3d_device = render_system_->GetDevice();
  if (sampler_state_) {
    SAFE_RELEASE(sampler_state_);
  }
  D3D11_SAMPLER_DESC sampler_desc;
  ZeroMemory(&sampler_desc, sizeof(sampler_desc));
  sampler_desc.Filter = TranslateSamplerState(sampler_state);
  sampler_desc.AddressU = TranslateTexWrapMode(sampler_state.wrap_u);
  sampler_desc.AddressV = TranslateTexWrapMode(sampler_state.wrap_v);
  sampler_desc.AddressW = TranslateTexWrapMode(sampler_state.wrap_w);
  sampler_desc.ComparisonFunc = TranslateCompareFunc(sampler_state.compare_func);
  sampler_desc.MaxAnisotropy = sampler_state.max_anisotropy;
  sampler_desc.MipLODBias = 0.0f;
  sampler_desc.MinLOD = 0;
  sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
  sampler_desc.BorderColor[0] = sampler_state.border_color.x;
  sampler_desc.BorderColor[1] = sampler_state.border_color.y;
  sampler_desc.BorderColor[2] = sampler_state.border_color.z;
  sampler_desc.BorderColor[3] = sampler_state.border_color.w;
  HRESULT hr = d3d_device->CreateSamplerState(&sampler_desc, &sampler_state_);
  HRESULT_HANDLE(hr, ERROR, "CreateSamplerState failed ");

  D3D11_SAMPLER_DESC sampler_desc2;
  sampler_state_->GetDesc(&sampler_desc2);
  return true;
}

void D3DTexture::UseForStage(RenderPipelineStage stage, int index,
                             D3DRenderer* renderer) {
  DCHECK(NULL != render_system_);
  if (stage == kVertexStage) {
    SetVSSampler(index, renderer);
  } else if (stage == kHullStage) {
    SetHSSampler(index, renderer);
  } else if (stage == kDomainStage) {
    SetDSSampler(index, renderer);
  } else if (stage == kGeometryStage) {
    SetGSSampler(index, renderer);
  } else if (stage == kPixelStage) {
    SetPSSampler(index, renderer);
  } else {
    CHECK(false);
  }
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

  ID3D11DeviceContext* d3d_context = render_system_->GetContext();
  d3d_context->CopyResource(tex->texres_, texres_);
  return true;
}


// class D3DTexture2DShared
ID3D11Resource* D3DTexture2DShared::GetSharedResource() {
  DCHECK(shared_texres_ != NULL);
  return shared_texres_;
}

HANDLE D3DTexture2DShared::GetSharedHanle() {
  DCHECK(shared_handle_ != NULL);
  return shared_handle_;
}

bool ValidTextureFlags(const Texture::Options& opt) {
  // cpu access require for Usage
  // reference D3D11_CPU_ACCESS_FLAG enumeration
  if (opt.cpu_access & kCPURead) {
    if (!(opt.usage & kBufferStaging)) {
      return false;
    }

    if (opt.target & kBindTargetRenderTarget) {
      return false;
    }

    if (opt.target & kBindTargetShaderResource) {
      return false;
    }
  }
  if (opt.cpu_access & kCPUWrite) {
    if (!(opt.usage & kBufferStaging
          && opt.usage & kBufferDynamic)) {
      return false;
    }
  }

  return true;
}

// class D3DTexture2D

void D3DTexture2D::ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) {
}

bool D3DTexture2D::InitFromImage(const Image* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  const ImageDataPtr& data = image->data(0);
  uint32 expect_size = SizeofDataFormat(options_.format)
      * options_.size.width() * options_.size.height();
  if (data->data_size() != static_cast<int32>(expect_size)) {
    LOG(ERROR) << "unexpected size: " << data->data_size()
               << " expected: " << expect_size;
    return false;
  }
  
  
  D3D11_SUBRESOURCE_DATA subres;
  subres.pSysMem = data->data();
  subres.SysMemPitch = options_.size.width() * SizeofDataFormat(options_.format);
  subres.SysMemSlicePitch = 0;  // no meaning for 2D
  return Init(&subres, 1);
}

bool D3DTexture2D::InitUnorderedAccessView() {
  ID3D11Device* d3d_device = render_system_->GetDevice();
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  desc.Format = tex_desc_.Format;
  desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipSlice = 0;
  HRESULT hr = d3d_device->CreateUnorderedAccessView(texres_, &desc, &uav_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateUnorderedAccessView failed ");
  return true;
}

void D3DTexture2D::InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  DCHECK(texres_ != NULL);
  DCHECK_EQ(GetViewDimensionFromTextureType(options_.type),
            D3D11_SRV_DIMENSION_TEXTURE2D);
  desc->Format = tex_desc_.Format;
  desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  desc->Texture2D.MipLevels = static_cast<UINT>(-1); // tex_desc_.MipLevels;
  desc->Texture2D.MostDetailedMip = 0;
}

// reference: MSDN, Surface Sharing Between Windows Graphics APIs
D3DTexture2DShared::D3DTexture2DShared(const Texture::Options& opt,
                                       D3DRenderSystem* rs)
    : D3DTexture2D(opt, rs)
    , shared_handle_(NULL) {
  DCHECK(opt.target & kBindTargetRenderTarget);
  DCHECK_EQ(opt.usage, kBufferDefault);
}

D3DTexture2DShared::~D3DTexture2DShared() {
  if (shared_handle_ != NULL) {
    // CloseHandle(shared_handle_);
  }
}

void D3DTexture2DShared::ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) {
  desc->MiscFlags      |= D3D11_RESOURCE_MISC_SHARED;
}

bool D3DTexture2DShared::InitSharedResource() {
  CHECK(NULL == shared_handle_);
  HRESULT hr;
  IDXGIResource* dxgi_res = NULL;
  ID3D11Resource* resource = GetResource();
  hr = resource->QueryInterface(__uuidof(IDXGIResource), (void**)&dxgi_res);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get IDXGIResource";
    return false;
  }

  hr = dxgi_res->GetSharedHandle(&shared_handle_);
  SAFE_RELEASE(dxgi_res);
  if (FAILED(hr)) {
    return false;
  }

  ID3D11Texture2D* shared_tex = NULL;
  ID3D11Device* d3d_device = render_system_->GetDevice();
  hr = d3d_device->OpenSharedResource(shared_handle_,
                                      __uuidof(ID3D11Texture2D),
                                      (void**)&shared_texres_);
  if (FAILED(hr)) {
    SAFE_RELEASE(shared_texres_);
    return false;
  }

  return true;
}

bool D3DTexture2DShared::Init(const D3D11_SUBRESOURCE_DATA* data, int num) {
  if (!D3DTexture::Init(data, num)) {
    return false;
  }

  return InitSharedResource();
}

D3DTexture2DExtern* D3DTexture2DExtern::Create(HANDLE handle,
                                               D3DRenderSystem* rs) {
  ID3D11Texture2D* shared_tex = NULL;
  ID3D11Device* d3d_device = rs->GetDevice();
  HRESULT hr = d3d_device->OpenSharedResource(handle, __uuidof(ID3D11Texture2D),
                                              (void**)&shared_tex);
  if (FAILED(hr)) {
    SAFE_RELEASE(shared_tex);
    return NULL;
  }

  D3D11_TEXTURE2D_DESC desc;
  shared_tex->GetDesc(&desc);
  Texture::Options opt;
  opt.size = gfx::Size(desc.Width, desc.Height);
  opt.format = TranslateD3DFormat(desc.Format);
  std::unique_ptr<D3DTexture2DExtern> ptr(new D3DTexture2DExtern(opt, rs));
  ptr->Attach(shared_tex);
  if (ptr->GetResource()) {
    return ptr.release();
  } else {
    return NULL;
  }
}

void D3DTexture2DExtern::Attach(ID3D11Texture2D* tex) {
  D3DTexture::Attach(tex);
  tex->GetDesc(&tex_desc_);
  InitResourceView();
}

void D3DTexture2DExtern::ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) {
  desc->MiscFlags      = D3D11_RESOURCE_MISC_SHARED;
}

// class D3D11TextureCubeMap
D3DTextureCubeMap::D3DTextureCubeMap(const Texture::Options& opt, 
                                     D3DRenderSystem* rs)
    : D3DTexture(opt, rs) {
}

void D3DTextureCubeMap::ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) {
  DCHECK_EQ(desc->Width, desc->Height) << "Cubemap's width must equal height.";
  desc->MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
} 

bool D3DTextureCubeMap::InitFromImage(const Image* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  const ImageDataPtr& data = image->data(0);
  uint32 expect_size = SizeofDataFormat(options_.format)
      * options_.size.width() * options_.size.height();
  if (data->data_size() != static_cast<int32>(expect_size)) {
    LOG(ERROR) << "unexpected size: " << data->data_size()
               << " expected: " << expect_size;
    return false;
  }
  
  D3D11_SUBRESOURCE_DATA subres[6];
  for (int i = 0; i < 6; ++i) {
    const ImageDataPtr& data = image->data(i);

    subres[i].pSysMem = data->data();
    subres[i].SysMemPitch = options_.size.width()
        * SizeofDataFormat(options_.format);
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres, 6);
}

void D3DTextureCubeMap::InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  DCHECK(texres_ != NULL);
  DCHECK_EQ(GetViewDimensionFromTextureType(options_.type),
            D3D11_SRV_DIMENSION_TEXTURECUBE);
  memset(desc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
  desc->Format = tex_desc_.Format;
  desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
  desc->TextureCube.MipLevels = tex_desc_.MipLevels;
  desc->TextureCube.MostDetailedMip = 0;
}


// class D3D11TextureCubeMap
D3DTexture2DArray::D3DTexture2DArray(const Texture::Options& opt, 
                                     D3DRenderSystem* rs)
    : D3DTexture(opt, rs),
      diminison_(-1) {
}

void D3DTexture2DArray::ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) {
  DCHECK_EQ(desc->Width, desc->Height) << "Cubemap's width must equal height.";
  desc->MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
} 

bool D3DTexture2DArray::InitFromImage(const Image* image) {
  // [reference] MSDN: How to: Initialize a Texture Programmatically
  const ImageDataPtr& data = image->data(0);
  uint32 expect_size = SizeofDataFormat(options_.format)
      * options_.size.width() * options_.size.height();
  if (data->data_size() != static_cast<int32>(expect_size)) {
    LOG(ERROR) << "unexpected size: " << data->data_size()
               << " expected: " << expect_size;
    return false;
  }
  
  D3D11_SUBRESOURCE_DATA subres[kTex2DArrayMaxDepth] = { 0 };
  CHECK_LT(image->depth(), kTex2DArrayMaxDepth);
  diminison_ = image->depth();
  for (int i = 0; i < image->depth(); ++i) {
    const ImageDataPtr& data = image->data(i);
    subres[i].pSysMem = data->data();
    subres[i].SysMemPitch = data->width() * SizeofDataFormat(options_.format);
    subres[i].SysMemSlicePitch = 0;  // no meaning for 2D
  }
  return Init(subres, image->depth());
}

void D3DTexture2DArray::InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) {
  DCHECK(texres_ != NULL);
  DCHECK_EQ(GetViewDimensionFromTextureType(options_.type),
    D3D11_SRV_DIMENSION_TEXTURE2DARRAY);
  memset(desc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
  desc->Format = tex_desc_.Format;
  desc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  desc->Texture2DArray.MipLevels = tex_desc_.MipLevels;
  desc->Texture2DArray.MostDetailedMip = 0;
  desc->Texture2DArray.FirstArraySlice = 0;
  desc->Texture2DArray.ArraySize = diminison_;
}

}  // namespace d3d11
}  // namespace azer
