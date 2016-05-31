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
  memset(input_, 0, sizeof(input_));
  memset(output_, 0, sizeof(output_));
  input_count_ = 0;
  output_count_ = 0;
}

void GpuComputeTask::SetInputTexture(int32_t index, TextureView* tex) {
  DCHECK_LT(index, input_count_);
  input_[index] = tex;
}
void GpuComputeTask::SetOutputTexture(int32_t index, TextureView* tex) {
  DCHECK_LT(index, output_count_);
  output_[index] = tex;
}

TextureView* GpuComputeTask::GetInputAt(int32_t index) {
  return input_[index];
}

TextureView* GpuComputeTask::GetOutputAt(int32_t index) {
  return output_[index];
}

GpuComputeTaskDispatcher::GpuComputeTaskDispatcher() {}
}  // namespace azer
