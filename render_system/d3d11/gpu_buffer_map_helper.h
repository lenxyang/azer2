#pragma once

#include "azer/render/gpu_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
class GpuBufferMapHelper {
 public:
  GpuBufferMapHelper(const GpuBufferOptions& options, ID3D11Buffer* buffer);
  ~GpuBufferMapHelper();
  GpuBufferLockDataPtr map(MapType flags);
  void unmap();
 private:
  bool locked_;
  ID3D11Buffer* buffer_;
  const GpuBufferOptions options_;
  DISALLOW_COPY_AND_ASSIGN(GpuBufferMapHelper);
};
}  // namespace d3d11
}  // namespace azer
