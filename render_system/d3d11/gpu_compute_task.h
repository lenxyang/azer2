#pragma once

#include "azer/render/gpu_compute_task.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {

class D3DGpuComputeTaskDispatcher : public GpuComputeTaskDispatcher {
 public:
  D3DGpuComputeTaskDispatcher();
  void Reset() override;
  void Dispatch(GpuComputeTask* task, const GpuTaskParams params) override;
 private:
  //
  ID3D11ShaderResourceView* resview_[GpuComputeTask::kMaxInput];
  ID3D11UnorderedAccessView* uavview_[GpuComputeTask::kMaxOutput];
  DISALLOW_COPY_AND_ASSIGN(D3DGpuComputeTaskDispatcher);
};
}  // namespace d3d11
}  // namespace azer
