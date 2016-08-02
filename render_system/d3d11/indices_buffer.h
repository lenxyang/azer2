#pragma once

#include "azer/render/common.h"
#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class GpuResLockHelper;

class D3DIndicesBuffer : public IndicesBuffer {
 public:
  D3DIndicesBuffer(const GpuResOptions& opt);
  ~D3DIndicesBuffer() override;

  void SetName(const std::string& name) override;
  GpuResLockDataPtr map(MapType flags) override;
  void unmap() override;
  bool CopyTo(GpuResource* res) override;
  NativeGpuResourceHandle native_handle() override {
    return (NativeGpuResourceHandle)buffer();
  }

  bool Initialized() { return NULL != buffer_;}
  ID3D11Buffer* buffer() { return buffer_;}

  bool Init(const IndicesData* data, D3DRenderSystem* rs);
 private:
  bool locked_;
  ID3D11Buffer* buffer_;
  std::unique_ptr<GpuResLockHelper> map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DIndicesBuffer);
};
}  // namespace d3d11
}  // namespace azer
