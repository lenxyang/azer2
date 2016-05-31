#pragma once

#include <d3d11.h>
#include "azer/render/texture_view.h"

namespace azer {
namespace d3d11 {
class D3DTextureView : public TextureView {
 public:
  D3DTextureView(const Options& options, Texture* tex);
  virtual ~D3DTextureView();
  virtual bool Init() = 0;
  void GenerateMips(int32_t level) override;
 protected:
  bool CheckTexFormatCapability();
 private:
  DISALLOW_COPY_AND_ASSIGN(D3DTextureView);
};

class D3DResTextureView : public D3DTextureView {
 public:
  D3DResTextureView(const Options& options, Texture* tex);
  ~D3DResTextureView() override;

  bool Init() override;
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  void GenerateMips(int32_t level) override;
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DResTextureView);
};

class D3DUAResTextureView : public D3DTextureView {
 public:
  D3DUAResTextureView(const Options& options, Texture* tex);
  ~D3DUAResTextureView() override;

  bool Init() override;
  ID3D11UnorderedAccessView* GetResourceView() { return uav_view_;}
 private:
  ID3D11UnorderedAccessView* uav_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUAResTextureView);
};
}  // namespace d3d11
}  // namespace azer
