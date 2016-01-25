#include "azer/render_system/d3d11/gpu_compute_task.h"

namespace azer {
namespace d3d11 {
D3DGpuComputeTask::D3DGpuComputeTask(const ShaderInfo& info)
    : GpuComputeTask(info) {
}

D3DGpuComputeTask::~D3DGpuComputeTask() {
}

void D3DGpuComputeTask::SetInputTexture(int32 index, Texture* tex) {
}

void D3DGpuComputeTask::SetOutputTexture(int32 index, Texture* tex) {
}

void D3DGpuComputeTask::Dispatch(uint32 thread_group_x, uint32 thread_group_y,
                                 uint32 thread_group_z) {
}
}  // namespace d3d11
}  // namespace azer
