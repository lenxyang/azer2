#include "azer/render/shader_closure.h"

#include "azer/render/gpu_resource.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/renderer.h"
#include "azer/render/shader.h"
#include "azer/render/shader_param_table.h"
#include "azer/render/texture.h"

namespace azer {
ShaderClosure::ShaderClosure(RenderPipelineStage stage)
    : stage_(stage) {
}

ShaderParamTable* ShaderClosure::table_at(int index) {
  return table_[index].get();
}

void ShaderClosure::SetShader(Shader* shader, int table_count, int res_count, 
                              int uares_count) {
  shader_ = shader;
  table_.resize(table_count);
  shader_res_.resize(res_count);
  shader_uares_.resize(uares_count);
}

void ShaderClosure::SetResource(int index, ShaderResView* tex) {
  CHECK_LT(index, shader_res_.size());
  shader_res_[index] = tex;
}

void ShaderClosure::SetUAResource(int index, UnorderAccessView* tex) {
  CHECK_LT(index, shader_uares_.size());
  shader_uares_[index] = tex;
}

void ShaderClosure::SetShaderParamTable(int index, ShaderParamTable* table) {
  CHECK_LT(index, table_.size());
  table_[index] = table;
}

void ShaderClosure::UpdateShaderParam(Renderer* renderer) {
  for (auto iter = table_.begin(); iter != table_.end(); ++iter) {
    (*iter)->flush(renderer);
  }
}

void ShaderClosure::Bind(Renderer* renderer) {
  UpdateShaderParam(renderer);
  if (!shader_res_.empty()) {
    renderer->SetShaderResource(
        stage(), 0, static_cast<int>(shader_res_.size()), &shader_res_.front());
  }
  if (!shader_uares_.empty()) {
    renderer->SetShaderUAResource(
        stage(), 0, static_cast<int>(shader_uares_.size()), &shader_uares_.front());
  }

  for (int i = 0; i < table_.size(); ++i) {
    renderer->BindShaderParamTable(stage(), i, table_[i].get());
  }
  renderer->SetShader(stage(), shader_.get());
}

void ShaderClosure::Reset(Renderer* renderer) {
  renderer->ResetStageResource(stage());
  renderer->ResetShader(stage());
}
}  // namespace azer
