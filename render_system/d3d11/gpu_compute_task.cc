#include "azer/render_system/d3d11/gpu_compute_task.h"

#include "azer/render/vertex_buffer.h"
#include "azer/render/gpu_program.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
D3DGpuComputeTaskDispatcher::D3DGpuComputeTaskDispatcher() {
}

void D3DGpuComputeTaskDispatcher::Reset() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11ShaderResourceView* null_srv[] = {NULL};
  ID3D11UnorderedAccessView* null_uav[] = {NULL};
  ID3D11DeviceContext* context = rs->GetContext();
  context->CSSetShaderResources(0, 1, null_srv);
  context->CSSetUnorderedAccessViews(0, 1, null_uav, 0);
}

void D3DGpuComputeTaskDispatcher::Dispatch(GpuComputeTask* task,
                                           const GpuTaskParams params) {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11DeviceContext* context = rs->GetContext();
  context->Dispatch(params.thread_group_x, params.thread_group_y,
                    params.thread_group_z);
  Reset();
}
}  // namespace d3d11
}  // namespace azer
