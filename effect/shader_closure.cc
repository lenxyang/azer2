#include "azer/effect/shader_closure.h"

#include "azer/render/gpu_constants_table.h"
#include "azer/render/gpu_resource.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/renderer.h"
#include "azer/render/shader.h"
#include "azer/render/texture.h"

namespace azer {
ShaderClosure::ShaderClosure(RenderPipelineStage stage)
    : stage_(stage) {
}

GpuConstantsTable* ShaderClosure::table_at(int index) {
  return table_[index].get();
}

void ShaderClosure::SetShader(Shader* shader, int table_count, int res_count, 
                              int uares_count) {
  shader_ = shader;
  table_.resize(table_count);
  shader_res_.resize(res_count);
  shader_uares_.resize(uares_count);
}

void ShaderClosure::SetResource(int index, int count, ShaderResView* tex) {
  CHECK_LT(index + count, shader_res_.size());
  ShaderResViewPtr* cur = &shader_res_.front() + index;
  for (int i = 0; i < count; ++i, ++cur) {
    (*cur) = tex;
  }
}

void ShaderClosure::SetUAResource(int index, int count, UnorderAccessResView* tex) {
  CHECK_LT(index + count, shader_uares_.size());
  UnorderAccessResViewPtr* cur = &shader_uares_.front() + index;
  for (int i = 0; i < count; ++i, ++cur) {
    (*cur) = tex;
  }
}

void ShaderClosure::SetGpuConstantsTable(int index, int count, 
                                         GpuConstantsTable* table) {
  CHECK_LT(index + count, table_.size());
  GpuConstantsTablePtr* cur = &table_.front() + index;
  for (int i = 0; i < count; ++i, ++cur) {
    (*cur) = table;
  }
}

void ShaderClosure::UpdateShaderParams(Renderer* renderer) {
  for (auto iter = table_.begin(); iter != table_.end(); ++iter) {
    (*iter)->flush(renderer);
  }
}

void ShaderClosure::Bind(Renderer* renderer) {
  if (!shader_res_.empty()) {
    renderer->SetShaderResource(
        stage(), 0, static_cast<int>(shader_uares_.size()), &shader_res_.front());
  }
  if (!shader_uares_.empty()) {
    renderer->SetShaderUAResource(
        stage(), 0, static_cast<int>(shader_uares_.size()), &shader_uares_.front());
  }

  for (int i = 0; i < table_.size(); ++i) {
    renderer->BindConstantsTable(stage(), i, table_[i].get());
  }
  renderer->SetShader(stage(), shader_.get());
}

void ShaderClosure::Reset(Renderer* renderer) {
  renderer->ResetStageResource(stage());
  renderer->ResetShader(stage());
}
}  // namespace azer
