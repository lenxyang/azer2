#pragma once

#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/structured_buffer.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class GpuResLockHelper;

class D3DStructuredGpuBuffer : public StructuredGpuBuffer {
 public:
  D3DStructuredGpuBuffer(const GpuResOptions& options, int count, int strip);
  ~D3DStructuredGpuBuffer() override;

  GpuResLockDataPtr map(MapType flags) override;
  void unmap() override;
  ID3D11Buffer* object() { return bufobj_;}

  bool Init(D3DRenderSystem* rs);
  NativeGpuResourceHandle native_handle() override {
    return (NativeGpuResourceHandle)object();
  }
 private:
  ID3D11Buffer* bufobj_;
  std::unique_ptr<GpuResLockHelper> map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DStructuredGpuBuffer);
};

class D3DStructuredBufferResView: public ShaderResView {
 public:
  D3DStructuredBufferResView(GpuResource* buffer);
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
  D3DUAStructuredBufferResView(GpuResource* buffer);
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
