
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
  D3DTexture(const Options& opt, D3DRenderSystem* rs);

  virtual ~D3DTexture();
  virtual bool Init(const D3D11_SUBRESOURCE_DATA* data, int arraysize, int mipmap);
  MapData map(MapType type) override;
  void unmap() override;

  bool Initialized() const { return NULL != texres_;}
  void Attach(ID3D11Texture2D* tex) { texres_ = tex;}
  void Detach() { texres_ = NULL; }

  ID3D11Resource* GetResource() { return texres_;}
  int32 diminison() const { return diminison_;}
  bool CopyTo(Texture* texture) override;
  const D3D11_TEXTURE2D_DESC& desc() const { return tex_desc_;}
 protected:
  ID3D11Resource* texres_;
  D3DRenderSystem* render_system_;
  D3D11_TEXTURE2D_DESC tex_desc_;
  int32 diminison_;

#ifdef DEBUG
  bool mapped_;
#endif
  DISALLOW_COPY_AND_ASSIGN(D3DTexture);
};

class D3DTexture2D : public D3DTexture {
 public:
  D3DTexture2D(const Options& opt, D3DRenderSystem* rs)
      : D3DTexture(opt, rs) {
  }

  bool InitFromImage(const ImageData* image) override;
 protected:
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2D);
};

class D3DTextureCubeMap : public D3DTexture {
 public:
  D3DTextureCubeMap(const Options& opt, D3DRenderSystem* rs);
  bool InitFromImage(const ImageData* image) override;
  bool InitTexture();
 protected:
  DISALLOW_COPY_AND_ASSIGN(D3DTextureCubeMap);
};

class D3DTexture2DArray : public D3DTexture {
 public:
  D3DTexture2DArray(const Options& opt, D3DRenderSystem* rs);
  bool InitFromImage(const ImageData* image) override;
  bool InitTexture();
 protected:
  DISALLOW_COPY_AND_ASSIGN(D3DTexture2DArray);
};

}  // namespace d3d11
}  // namespace azer
