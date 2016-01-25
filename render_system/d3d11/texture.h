
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
  void GenerateMips(int level) override;
  bool SetSamplerState(const SamplerState& sampler_state) override;

  MapData map(MapType type) override;
  void unmap() override;

  void UseForStage(RenderPipelineStage stage, int index, D3DRenderer* renderer);
  void SetVSSampler(int index, D3DRenderer* renderer);
  void SetHSSampler(int index, D3DRenderer* renderer);
  void SetDSSampler(int index, D3DRenderer* renderer);
  void SetGSSampler(int index, D3DRenderer* renderer);
  void SetPSSampler(int index, D3DRenderer* renderer);

  bool Initialized() const { return NULL != texres_;}
  void Attach(ID3D11Texture2D* tex) { texres_ = tex;}
  void Detach() { texres_ = NULL; }

  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  ID3D11UnorderedAccessView* GetUnorderedAccessView() { return uav_view_;}
  ID3D11Resource* GetResource() { return texres_;}

  bool CopyTo(Texture* texture) override;
 protected:
  virtual void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) = 0;
  virtual void InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) = 0;
  virtual bool InitUnorderedAccessView() {return false;};
  bool InitResourceView();

  ID3D11Resource* texres_;
  D3DRenderSystem* render_system_;
  D3D11_TEXTURE2D_DESC tex_desc_;
  ID3D11SamplerState* sampler_state_;
  ID3D11ShaderResourceView* res_view_;
  ID3D11UnorderedAccessView* uav_view_;

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

  bool InitFromImage(const Image* image) override;
 protected:
  void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  void InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) override;
  bool InitUnorderedAccessView() override;
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2D);
};

// for shared resource with other D3DDevice
// here, is use to create texture for ANGLE using
class D3DTexture2DShared : public D3DTexture2D {
 public:
  D3DTexture2DShared(const Texture::Options& opt, D3DRenderSystem* rs);
  ~D3DTexture2DShared() override;

  // create a resource for other device using
  bool Init(const D3D11_SUBRESOURCE_DATA* data, int num) override;
  ID3D11Resource* GetSharedResource();
  HANDLE GetSharedHanle();
 protected:
  void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  bool InitFromImage(const Image* image) override { return false;}

  bool InitSharedResource();
  HANDLE shared_handle_;
  ID3D11Resource* shared_texres_;
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
  void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  bool InitFromImage(const Image* image) override { return false;}
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2DExtern);
};

class D3DTextureCubeMap : public D3DTexture {
 public:
  D3DTextureCubeMap(const Texture::Options& opt, D3DRenderSystem* rs);
  bool InitFromImage(const Image* image) override;
  bool InitTexture();
 protected:
  void ModifyTextureDesc(D3D11_TEXTURE2D_DESC* desc) override;
  void InitResourceDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc) override;
  DISALLOW_COPY_AND_ASSIGN(D3DTextureCubeMap);
};
}  // namespace d3d11
}  // namespace azer
