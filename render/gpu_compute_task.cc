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
  gpu_program_ = RenderSystem::Current()->CreateShader(info);
  Reset();
}

GpuComputeTask::~GpuComputeTask() {}

void GpuComputeTask::Reset() {
  memset(input_, 0, sizeof(input_));
  memset(output_, 0, sizeof(output_));
  input_count_ = 0;
  output_count_ = 0;
}

void GpuComputeTask::SetInputTexture(int32 index, Texture* tex) {
  DCHECK_LT(index, input_count_);
  input_[index] = tex;
}
void GpuComputeTask::SetOutputTexture(int32 index, Texture* tex) {
  DCHECK_LT(index, output_count_);
  output_[index] = tex;
}

Texture* GpuComputeTask::GetInputAt(int32 index) {
  return input_[index];
}

Texture* GpuComputeTask::GetOutputAt(int32 index) {
  return output_[index];
}

GpuComputeTaskDispatcher::GpuComputeTaskDispatcher() {}
}  // namespace azer
