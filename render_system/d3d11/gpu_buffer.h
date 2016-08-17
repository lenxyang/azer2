#pragma once

#include "azer/render/gpu_resource_view.h"
#include "azer/render/gpu_buffer.h"
#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;

class D3DGpuBuffer : public GpuBuffer {
 public:
  D3DGpuBuffer(const GpuResOptions& options, int count, int strip);
  ~D3DGpuBuffer() override;

  void SetName(const std::string& name) override;
  GpuResLockDataPtr map(MapType flags) override;
  void unmap() override;
  void UpdateData(const uint8_t* data, int64_t size) override;
  bool CopyTo(GpuResource* res) override;
  ID3D11Buffer* object() { return bufobj_;}

  bool Init(D3DRenderSystem* rs, const uint8_t* data);
  NativeGpuResourceHandle native_handle() override {
    return (NativeGpuResourceHandle)object();
  }
 private:
  ID3D11Buffer* bufobj_;
  GpuResLockHelperPtr map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DGpuBuffer);
};

class D3DBufferResView: public ShaderResView {
 public:
  D3DBufferResView(GpuResource* buffer);
  ~D3DBufferResView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
  NativeGpuResourceViewHandle native_handle() override {
    return (NativeGpuResourceViewHandle)GetResourceView();
  }
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DBufferResView);
};

class D3DBufferUAView: public UnorderAccessView {
 public:
  D3DBufferUAView(GpuResource* buffer);
  ~D3DBufferUAView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11UnorderedAccessView* GetResourceView() { return unorder_view_;}
  NativeGpuResourceViewHandle native_handle() override {
    return (NativeGpuResourceViewHandle)GetResourceView();
  }
 private:
  ID3D11UnorderedAccessView* unorder_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DBufferUAView);
};
}  // namespace d3d11
}  // namespace azer
