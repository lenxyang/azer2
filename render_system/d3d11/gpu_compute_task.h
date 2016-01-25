#pragma once

#include <d3d11.h>
#include "azer/render/gpu_compute_task.h"

namespace azer {
namespace d3d11 {

class D3DGpuComputeTaskDispatcher : public GpuComputeTaskDispatcher
 public:
  D3DGpuComputeTaskDispatcher();
  void Reset() override;
  void Dispatch(GpuComputeTask* task, TaskParams params) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(D3DGpuComputeTaskDispatcher);
};
}  // namespace d3d11
}  // namespace azer
