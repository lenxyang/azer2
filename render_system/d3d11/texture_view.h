#pragma once

#include "azer/render/texture_view.h"

namespace azer {
namespace d3d11 {
class D3DTexView : public TextureView {
 public:
  D3DTexView();
  ~D3DTexView() override;
  virtual bool Init() = 0;
  void GenerateMips(int32 level) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(D3DTexView);
};

class D3DResTextureView : public D3DTexView {
 public:
  D3DResTextureView();
  ~D3DResTextureView() override;

  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  void GenerateMips(int32 level) override;
 private:
  void InitRes();
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DResTextureView);
};

class D3DUAResTextureView : public TextureView {
 public:
  D3DUAResTextureView();
  ~D3DUAResTextureView() override;
  ID3D11UnorderedAccessView* GetResourceView() { return uav_view_;}
 private:
  ID3D11UnorderedAccessView* uav_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUAResTextureView);
};
}  // namespace d3d11
}  // namespace azer
