#include "azer/render_system/d3d11/gpu_compute_task.h"

#include "azer/render/vertex_buffer.h"
#include "azer/render/gpu_program.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/gpu_program.h"

namespace azer {
namespace d3d11 {
D3DGpuComputeTaskDispatcher::D3DGpuComputeTaskDispatcher() {
  memset(resview_, 0, sizeof(resview_));
  memset(uavview_, 0, sizeof(uavview_));
}

void D3DGpuComputeTaskDispatcher::Reset() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11ShaderResourceView* null_srv[] = {NULL};
  ID3D11UnorderedAccessView* null_uav[] = {NULL};
  ID3D11DeviceContext* context = rs->GetContext();
  context->CSSetShaderResources(0, 1, null_srv);
  context->CSSetUnorderedAccessViews(0, 1, null_uav, 0);
  context->CSSetShader(0, 0, 0);
}

void D3DGpuComputeTaskDispatcher::Dispatch(GpuComputeTask* task,
                                           const GpuTaskParams params) {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11DeviceContext* context = rs->GetContext();
  D3DComputeGpuProgram* program = (D3DComputeGpuProgram*)task->gpu_program();
  context->CSSetShader(program->resource(), 0, 0);
  context->CSSetShaderResources(0, task->input_count(), task->GetInput());
  context->CSSetUnorderedAccessViews(0, 1, task->output_count(), 
                                     task->GetOutput(), 0);
  context->Dispatch(params.thread_group_x, params.thread_group_y,
                    params.thread_group_z);
  Reset();
}
}  // namespace d3d11
}  // namespace azer
