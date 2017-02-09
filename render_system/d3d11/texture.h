#pragma once

#include "base/logging.h"
#include "azer/render/common.h"
#include "azer/render/texture.h"
#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"

namespace azer {
namespace d3d11 {
class D3DRenderer;
class D3DRenderSystem;

class D3DTexture2D: public Texture2D {
 public:
  D3DTexture2D(const Options& opt, D3DRenderSystem* rs);

  virtual ~D3DTexture2D();

  NativeGpuResourceHandle native_handle() override { return texres_;}
  void SetName(const std::string& name) override;
  bool Init(const ImageData* image) override;
  GpuResLockDataPtr map(MapType type) override;
  void unmap() override;
  void Attach(ID3D11Texture2D* tex);
  void Detach();
  
  bool CopyTo(GpuResource* texres) override;
  const D3D11_TEXTURE2D_DESC& desc() const { return texdesc_;}
 protected:
  bool InitFromData(const D3D11_SUBRESOURCE_DATA* data);
  ID3D11Texture2D* texres_;
  D3DRenderSystem* render_system_;
  D3D11_TEXTURE2D_DESC texdesc_;
  GpuResLockHelperPtr map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2D);
};

class D3DTexture2DArray : public Texture2DArray {
 public:
  D3DTexture2DArray(const Options& opt, D3DRenderSystem* rs);

  // common override
  NativeGpuResourceHandle native_handle() override { return texres_;}
  void SetName(const std::string& name) override;
  bool Init(const ImageData* image) override;
  GpuResLockDataPtr map(MapType type) override;
  void unmap() override;
  void Attach(ID3D11Texture2D* tex);
  void Detach();

  bool CopyTo(GpuResource* texres) override;
  const D3D11_TEXTURE2D_DESC& desc() const { return texdesc_;}
 protected:
  bool InitFromData(const D3D11_SUBRESOURCE_DATA* data);
  ID3D11Texture2D* texres_;
  D3D11_TEXTURE2D_DESC texdesc_;
  GpuResLockHelperPtr map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2DArray);
};

class D3DTextureCubeMap : public TextureCubemap {
 public:
  D3DTextureCubeMap(const Options& opt, D3DRenderSystem* rs);

  NativeGpuResourceHandle native_handle() override { return texres_;}
  void SetName(const std::string& name) override;
  bool Init(const ImageData* image) override;
  GpuResLockDataPtr map(MapType type) override;
  void unmap() override;
  void Attach(ID3D11Texture2D* tex);
  void Detach();

  bool CopyTo(GpuResource* texres) override;
  const D3D11_TEXTURE2D_DESC& desc() const { return texdesc_;}
 protected:
  bool InitFromData(const D3D11_SUBRESOURCE_DATA* data);
  ID3D11Texture2D* texres_;
  D3D11_TEXTURE2D_DESC texdesc_;
  GpuResLockHelperPtr map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DTextureCubeMap);
};

class D3DTexture3D: public Texture3D {
 public:
  D3DTexture3D(const Options& opt, D3DRenderSystem* rs);

  virtual ~D3DTexture3D();
  NativeGpuResourceHandle native_handle() override { return texres_;}
  void SetName(const std::string& name) override;
  GpuResLockDataPtr map(MapType type) override;
  void unmap() override;
  bool Init(const ImageData* image) override;
  
  void Attach(ID3D11Texture3D* tex);
  void Detach();

  bool CopyTo(GpuResource* texres) override { return false;}
  const D3D11_TEXTURE3D_DESC& desc() const { return texdesc_;}
 protected:
  bool InitFromData(const D3D11_SUBRESOURCE_DATA* data);
  ID3D11Texture3D* texres_;
  D3DRenderSystem* render_system_;
  D3D11_TEXTURE3D_DESC texdesc_;
  GpuResLockHelperPtr map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DTexture3D);
};

bool UtilityTextureCopyTo(Texture* src, GpuResource* destres);
}  // namespace d3d11
}  // namespace azer
