#include "azer/render/gpu_compute_task.h"

#include "azer/render/render_system.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/shader.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
GpuComputeTask::GpuComputeTask(const ShaderInfo& info, int rescount, int uacount)
    : shader_info_(info) {
  DCHECK_EQ(info.stage, kComputeStage);
  gpu_program_ = RenderSystem::Current()->CreateShader(info, NULL);
  shader_resource_.resize(rescount);
  shader_uaresource_.resize(uacount);
  Reset();
}

GpuComputeTask::~GpuComputeTask() {}

void GpuComputeTask::Reset() {
}

void GpuComputeTask::Bind(Renderer* renderer) {
}

void GpuComputeTask::SetResource(int index, ResourceView* tex) {
  CHECK_LT(index, static_cast<int>(shader_resource_.size()));
  shader_resource_[index] = tex;
}

void GpuComputeTask::SetUAResource(int index, ResourceView* tex) {
  CHECK_LT(index, static_cast<int>(shader_uaresource_.size()));
  shader_uaresource_[index] = tex;
}

// class GpuComputeTaskDispatcher
GpuComputeTaskDispatcher::GpuComputeTaskDispatcher() {}
}  // namespace azer
