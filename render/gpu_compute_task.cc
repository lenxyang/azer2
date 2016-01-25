#include "azer/render/gpu_compute_task.h"

#include "azer/render/gpu_constants_table.h"
#include "azer/render/gpu_program.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
GpuComputeTask::GpuComputeTask(const ShaderInfo& info)
    : shader_info_(info) {}
GpuComputeTask::~GpuComputeTask() {}
}  // namespace azer
