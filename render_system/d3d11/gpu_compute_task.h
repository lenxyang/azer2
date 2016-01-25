#pragma once

#include <d3d11.h>
#include "azer/render/gpu_compute_task.h"

namespace azer {
namespace d3d11 {
class AZER_EXPORT D3DGpuComputeTask : public GpuComputeTask {
 public:
  explicit D3DGpuComputeTask(const ShaderInfo& info);
  ~D3DGpuComputeTask() override;

  bool Init();

  void SetInputTexture(int32 index, Texture* tex) override;
  void SetOutputTexture(int32 index, Texture* tex) override;
  void Dispatch(uint32 thread_group_x, uint32 thread_group_y, uint32 thread_group_z)
      override; 
 private:
  DISALLOW_COPY_AND_ASSIGN(D3DGpuComputeTask);
};
}  // namespace d3d11
}  // namespace azer
