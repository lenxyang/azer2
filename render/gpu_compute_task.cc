#include "azer/render/gpu_compute_task.h"

#include "azer/render/render_system.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/gpu_program.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
GpuComputeTask::GpuComputeTask(const ShaderInfo& info)
    : shader_info_(info) {
  gpu_program_ = RenderSystem::Current()->CreateGpuProgram(info);
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
  DCHECK_LT(output, output_count_);
  output_[index] = tex;
}

GpuComputeTaskDispatcher::GpuComputeTaskDispatcher() {}
}  // namespace azer
