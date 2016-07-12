#include "azer/render_system/d3d11/gpu_compute_task.h"

#include "azer/render/vertex_buffer.h"
#include "azer/render/shader.h"
#include "azer/render_system/d3d11/shader.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/texture_view.h"

namespace azer {
namespace d3d11 {
D3DGpuComputeTaskDispatcher::D3DGpuComputeTaskDispatcher() {
  memset(resview_, 0, sizeof(resview_));
  memset(uavview_, 0, sizeof(uavview_));
}
  /*
void D3DGpuComputeTaskDispatcher::Reset() {
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11ShaderResourceView* null_srv[] = {NULL};
  ID3D11UnorderedAccessView* null_uav[] = {NULL};
  ID3D11DeviceContext* context = rs->GetContext();
  context->CSSetShaderResources(0, 1, null_srv);
  context->CSSetUnorderedAccessViews(0, 1, null_uav, 0);
  context->CSSetShader(0, 0, 0);
}
  */


void D3DGpuComputeTaskDispatcher::Dispatch(const GpuTaskParams params) {
  /*
  D3DComputeShader* program = (D3DComputeShader*)task->gpu_program();
  input_.Bind(renderer);
  for (int32_t i = 0; i < task->input_count(); ++i) {
    resview_[i] = ((D3DResTextureView*)task->GetInputAt(i))->GetResourceView();
    DCHECK(resview_[i] != NULL);
  }

  for (int32_t i = 0; i < task->output_count(); ++i) {
    uavview_[i] = ((D3DUAResTextureView*)task->GetOutputAt(i))->GetResourceView();
    DCHECK(uavview_[i] != NULL);
  }
  context->CSSetShader(program->resource(), 0, 0);
  context->CSSetShaderResources(0, task->input_count(), resview_);
  context->CSSetUnorderedAccessViews(0, task->output_count(), uavview_, 0);
  
  Reset();
  */
  D3DRenderSystem* rs = (D3DRenderSystem*)RenderSystem::Current();
  ID3D11DeviceContext* context = rs->GetContext();
  context->Dispatch(params.thread_group_x, params.thread_group_y,
                    params.thread_group_z);
}
}  // namespace d3d11
}  // namespace azer
