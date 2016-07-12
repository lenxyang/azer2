#pragma once

#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class GpuBufferMapHelper;

class D3DIndicesBuffer : public IndicesBuffer {
 public:
  D3DIndicesBuffer(const GpuBufferOptions& opt);
  ~D3DIndicesBuffer() override;

  GpuBufferLockDataPtr map(MapType flags) override;
  void unmap() override;

  bool Initialized() { return NULL != buffer_;}
  ID3D11Buffer* buffer() { return buffer_;}
  bool Init(const IndicesData* data, D3DRenderSystem* rs);
 private:
  bool locked_;
  ID3D11Buffer* buffer_;
  std::unique_ptr<GpuBufferMapHelper> map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DIndicesBuffer);
};
}  // namespace d3d11
}  // namespace azer
