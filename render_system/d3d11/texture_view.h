#pragma once

#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render/texture_view.h"

namespace azer {
namespace d3d11 {
class D3DResTextureView : public ShaderResView {
 public:
  D3DResTextureView(const Options& options, Texture* tex);
  ~D3DResTextureView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  void GenerateMips(int32_t level) override;
  NativeGpuResourceViewHandler native_handle() override {
    return (NativeGpuResourceViewHandler)GetResourceView();
  }
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DResTextureView);
};

class D3DUAResTextureView : public UATextureView {
 public:
  D3DUAResTextureView(const Options& options, Texture* tex);
  ~D3DUAResTextureView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11UnorderedAccessView* GetResourceView() { return uav_view_;}
  NativeGpuResourceViewHandler native_handle() override {
    return (NativeGpuResourceViewHandler)GetResourceView();
  }
 private:
  ID3D11UnorderedAccessView* uav_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUAResTextureView);
};
}  // namespace d3d11
}  // namespace azer
