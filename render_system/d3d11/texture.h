
#pragma once

#include <d3d11.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/render/texture.h"

namespace azer {
namespace d3d11 {
class D3DRenderer;
class D3DRenderSystem;

class D3DTexture: public Texture {
 public:
  D3DTexture(const Texture::Options& opt, D3DRenderSystem* rs);

  virtual ~D3DTexture();
  virtual bool Init(const D3D11_SUBRESOURCE_DATA* data, int num);
  virtual void GenerateMips(int level) override;
  virtual bool SetSamplerState(const SamplerState& sampler_state) override;

  virtual MapData map(MapType type) override;
  virtual void unmap() override;

  void UseForStage(RenderPipelineStage stage, int index, D3DRenderer* renderer);
  void SetVSSampler(int index, D3DRenderer* renderer);
  void SetPSSampler(int index, D3DRenderer* renderer);

  bool Initialized() const { return NULL != resource_;}
  void Attach(ID3D11Texture2D* tex) { resource_ = tex;}
  void Detach() { resource_ = NULL; }

  ID3D11ShaderResourceView* GetResourceView() { return view_;}
  ID3D11Resource* GetResource() { return resource_;}

  virtual bool CopyTo(Texture* texture) override;
 protected:
  virtual void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) = 0;
  virtual void InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) = 0;
  bool InitResourceView();

  ID3D11Resource* resource_;
  D3DRenderSystem* render_system_;
  ID3D11ShaderResourceView* view_;
  ID3D11SamplerState* sampler_state_;
  D3D11_SHADER_RESOURCE_VIEW_DESC res_view_desc_;
  D3D11_TEXTURE2D_DESC tex_desc_;

#ifdef DEBUG
  bool mapped_;
#endif
  DISALLOW_COPY_AND_ASSIGN(D3DTexture);
};

class D3DTexture2D : public D3DTexture {
 public:
  D3DTexture2D(const Texture::Options& opt, D3DRenderSystem* rs)
      : D3DTexture(opt, rs) {
  }

  virtual bool InitFromImage(const Image* image) override;
 protected:
  virtual void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  virtual void InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) override;
  friend class D3DRenderTarget;
  friend class D3DSurfaceRenderTarget;
  friend class D3DDepthBuffer;
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2D);
};

// for shared resource with other D3DDevice
// here, is use to create texture for ANGLE using
class D3DTexture2DShared : public D3DTexture2D {
 public:
  D3DTexture2DShared(const Texture::Options& opt, D3DRenderSystem* rs);
  virtual ~D3DTexture2DShared();

  // create a resource for other device using
  virtual bool Init(const D3D11_SUBRESOURCE_DATA* data, int num);
  ID3D11Resource* GetSharedResource();
  HANDLE GetSharedHanle();
 protected:
  virtual void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  virtual bool InitFromImage(const Image* image) { return false;}

  bool InitSharedResource();
  HANDLE shared_handle_;
  ID3D11Resource* shared_resource_;
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2DShared);
};

class D3DTexture2DExtern : public D3DTexture2D {
 public:
  D3DTexture2DExtern(const Texture::Options& opt, D3DRenderSystem* rs)
      : D3DTexture2D(opt, rs) {}

  // create from other device's resources
  static D3DTexture2DExtern* Create(HANDLE handle, D3DRenderSystem* rs);
 private:
  void Attach(ID3D11Texture2D* tex);
  virtual void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  virtual bool InitFromImage(const Image* image) { return false;}
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2DExtern);
};

class D3DTextureCubeMap : public D3DTexture {
 public:
  D3DTextureCubeMap(const Texture::Options& opt, D3DRenderSystem* rs)
      : D3DTexture(opt, rs) {
    
  }
  virtual bool InitFromImage(const Image* image) override;
 protected:
  virtual void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  virtual void InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) override;
  DISALLOW_COPY_AND_ASSIGN(D3DTextureCubeMap);
};

inline D3DTexture::D3DTexture(const Texture::Options& opt, D3DRenderSystem* rs)
    : Texture(opt)
    , render_system_(rs)
    , view_(NULL)
    , resource_(NULL)
    , sampler_state_(NULL) {
#ifdef DEBUG
  mapped_ = false;
#endif
}

inline D3DTexture::~D3DTexture() {
  SAFE_RELEASE(resource_);
  SAFE_RELEASE(view_);
  SAFE_RELEASE(sampler_state_);
}


inline ID3D11Resource* D3DTexture2DShared::GetSharedResource() {
  DCHECK(shared_resource_ != NULL);
  return shared_resource_;
}

inline HANDLE D3DTexture2DShared::GetSharedHanle() {
  DCHECK(shared_handle_ != NULL);
  return shared_handle_;
}
}  // namespace d3d11
}  // namespace azer
