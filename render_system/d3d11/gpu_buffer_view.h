#pragma once

#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render/gpu_buffer_view.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class GpuBufferMapHelper;

class D3DShaderResView: public ShaderResView {
 public:
  D3DShaderResView(const Options& opt, GpuBuffer* buffer);
  ~D3DShaderResView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  NativeGpuResourceViewHandler native_handle() override {
    return (NativeGpuResourceViewHandler)GetResourceView();
  }
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DShaderResView);
};

class D3DUnorderAccessResView: public UnorderAccessResView {
 public:
  D3DUnorderAccessResView(const Options& opt, GpuBuffer* buffer);
  ~D3DUnorderAccessResView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11UnorderedAccessView* GetResourceView() { return unorder_view_;}
  NativeGpuResourceViewHandler native_handle() override {
    return (NativeGpuResourceViewHandler)GetResourceView();
  }
 private:
  ID3D11UnorderedAccessView* unorder_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUnorderAccessResView);
};

}  // namespace d3d11
}  // namespace azer
