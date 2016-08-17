#pragma once

#include "azer/render/gpu_resource.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
class GpuResLockHelper : public ::base::RefCounted<GpuResLockHelper> {
 public:
  GpuResLockHelper(const GpuResOptions& options, ID3D11Resource* buffer);
  ~GpuResLockHelper();
  GpuResLockDataPtr map(MapType flags);
  void unmap();
 private:
  bool locked_;
  ID3D11Resource* buffer_;
  const GpuResOptions options_;
  DISALLOW_COPY_AND_ASSIGN(GpuResLockHelper);
};

typedef scoped_refptr<GpuResLockHelper> GpuResLockHelperPtr;
}  // namespace d3d11
}  // namespace azer
