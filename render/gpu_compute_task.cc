#include "azer/render/gpu_compute_task.h"

#include "azer/render/render_system.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/shader.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
GpuComputeTask::GpuComputeTask(const ShaderInfo& info)
    : shader_info_(info) {
  DCHECK_EQ(info.stage, kComputeStage);
  gpu_program_ = RenderSystem::Current()->CreateShader(info, NULL);
  Reset();
}

GpuComputeTask::~GpuComputeTask() {}

void GpuComputeTask::Reset() {
}

void GpuComputeTask::SetInputResource(int index, ResourceView* tex) {
  input_.SetResource(kComputeStage, index, tex);
}

void GpuComputeTask::SetInputUAResource(int index, ResourceView* tex) {
  uainput_.SetResource(kComputeStage, index, tex);
}

void GpuComputeTask::SetOutputTexture(int32_t index, TextureView* tex) {
  output_.SetResource(kComputeStage, index, tex);
}

GpuComputeTaskDispatcher::GpuComputeTaskDispatcher() {}
}  // namespace azer
