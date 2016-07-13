#pragma once

#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render/texture_view.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DResTextureView : public TextureView {
 public:
  D3DResTextureView(const Options& options, Texture* tex);
  ~D3DResTextureView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  void GenerateMips(int32_t level) override;
  NativeGpuResourceViewHandle native_handle() override {
    return (NativeGpuResourceViewHandle)GetResourceView();
  }
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DResTextureView);
};

class D3DTextureUAResView : public TextureUAView {
 public:
  D3DTextureUAResView(const Options& options, Texture* tex);
  ~D3DTextureUAResView() override;
  void GenerateMips(int32_t level) override;

  bool Init(D3DRenderSystem* rs);
  ID3D11UnorderedAccessView* GetResourceView() { return uav_view_;}
  NativeGpuResourceViewHandle native_handle() override {
    return (NativeGpuResourceViewHandle)GetResourceView();
  }
 private:
  ID3D11UnorderedAccessView* uav_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DTextureUAResView);
};
}  // namespace d3d11
}  // namespace azer
