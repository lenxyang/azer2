#pragma once

#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render/gpu_buffer_view.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class GpuBufferMapHelper;

class D3DStructuredBufferResView: public ShaderResView {
 public:
  D3DStructuredBufferResView(GpuBuffer* buffer);
  ~D3DStructuredBufferResView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  NativeGpuResourceViewHandle native_handle() override {
    return (NativeGpuResourceViewHandle)GetResourceView();
  }
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DStructuredBufferResView);
};

class D3DUAStructuredBufferResView: public UnorderAccessResView {
 public:
  D3DUAStructuredBufferResView(GpuBuffer* buffer);
  ~D3DUAStructuredBufferResView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11UnorderedAccessView* GetResourceView() { return unorder_view_;}
  NativeGpuResourceViewHandle native_handle() override {
    return (NativeGpuResourceViewHandle)GetResourceView();
  }
 private:
  ID3D11UnorderedAccessView* unorder_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUAStructuredBufferResView);
};

}  // namespace d3d11
}  // namespace azer
