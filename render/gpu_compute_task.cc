#include "azer/render/gpu_compute_task.h"

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/shader.h"
#include "azer/render/shader_param_table.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
GpuComputeTask::GpuComputeTask(const ShaderInfo& info, int table_count,
                               int rescount, int uacount)
    : shader_info_(info) {
  DCHECK_EQ(info.stage, kComputeStage);
  gpu_program_ = RenderSystem::Current()->CreateShader(info, NULL);
  CHECK(gpu_program_.get()) << "Failed to CreateShader [" << info.path << "]";
  shader_resource_.resize(rescount);
  shader_uaresource_.resize(uacount);
  constants_table_.resize(table_count);
}

GpuComputeTask::~GpuComputeTask() {}

void GpuComputeTask::Reset(Renderer* renderer) {
  renderer->ResetShader(kComputeStage);
  renderer->ResetStageResource(kComputeStage);
}

void GpuComputeTask::Bind(Renderer* renderer) {
  if (shader_uaresource_.size() > 0u) {
    renderer->SetShaderUAResource(kComputeStage, 0,
                                  static_cast<int>(shader_uaresource_.size()),
                                  &(shader_uaresource_[0]));
  } else {
    renderer->SetShaderUAResource(kComputeStage, 0, 1, NULL);
  }

  if (shader_resource_.size() > 0u) {
    renderer->SetShaderResource(kComputeStage, 0,
                                static_cast<int>(shader_resource_.size()),
                                &(shader_resource_[0]));
  } else {
    renderer->SetShaderResource(kComputeStage, 0, 1, NULL);
  }

  for (int i = 0; i < constants_table_.size(); ++i) {
    ShaderParamTable* table = constants_table_[i].get();
    renderer->BindShaderParamTable(kComputeStage, i, table);
  }

  DCHECK(gpu_program_.get()) << "Shader Program cannot be NULL";
  renderer->SetShader(kComputeStage, gpu_program_.get());
}

void GpuComputeTask::SetResource(int index, ShaderResView* tex) {
  CHECK_LT(index, static_cast<int>(shader_resource_.size()));
  shader_resource_[index] = tex;
}

void GpuComputeTask::SetUAResource(int index, UnorderAccessResView* tex) {
  CHECK_LT(index, static_cast<int>(shader_uaresource_.size()));
  shader_uaresource_[index] = tex;
}

ShaderParamTable* GpuComputeTask::constants_table(int i) {
  return constants_table_[i].get();
}

void GpuComputeTask::SetShaderParamTable(int index, ShaderParamTable* table) {
  constants_table_[index] = table;
}

}  // namespace azer
