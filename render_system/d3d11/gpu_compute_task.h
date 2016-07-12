#pragma once

#include "azer/render/gpu_compute_task.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {

class D3DGpuComputeTaskDispatcher : public GpuComputeTaskDispatcher {
 public:
  D3DGpuComputeTaskDispatcher();
  void Dispatch(const GpuTaskParams params) override;
 private:
  //
  ID3D11ShaderResourceView* resview_[GpuComputeTask::kMaxResourceCount];
  ID3D11UnorderedAccessView* uavview_[GpuComputeTask::kMaxResourceCount];
  DISALLOW_COPY_AND_ASSIGN(D3DGpuComputeTaskDispatcher);
};
}  // namespace d3d11
}  // namespace azer
