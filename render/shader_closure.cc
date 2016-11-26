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

void ShaderClosure::SetResource(int index, ShaderResViewPtr tex) {
  CHECK_LT(index, shader_res_.size());
  shader_res_[index] = tex;
}

void ShaderClosure::SetUAResource(int index, UnorderAccessViewPtr tex) {
  CHECK_LT(index, shader_uares_.size());
  shader_uares_[index] = tex;
}

void ShaderClosure::SetShaderParamTable(int index, ShaderParamTablePtr table) {
  CHECK_LT(index, table_.size());
  table_[index] = table;
}

void ShaderClosure::UpdateShaderParam(int index, Renderer* renderer) {
  DCHECK_LT(index, static_cast<int>(table_.size()));
  table_[index]->flush(renderer);
}

void ShaderClosure::UpdateAllShaderParam(Renderer* renderer) {
  for (auto iter = table_.begin(); iter != table_.end(); ++iter) {
    (*iter)->flush(renderer);
  }
}

void ShaderClosure::UpdateRes(Renderer* renderer) {
  if (!shader_res_.empty()) {
    renderer->SetShaderResource(
        stage(), 0, static_cast<int>(shader_res_.size()), &shader_res_.front());
  }
}
void ShaderClosure::UpdateUARes(Renderer* renderer) {
  if (!shader_uares_.empty()) {
    renderer->SetShaderUAResource(
        stage(), 0, static_cast<int>(shader_uares_.size()), &shader_uares_.front());
  }
}

void ShaderClosure::Bind(Renderer* renderer) {
  UpdateAllShaderParam(renderer);
  UpdateRes(renderer);
  UpdateUARes(renderer);

  for (int i = 0; i < static_cast<int>(table_.size()); ++i) {
    ShaderParamTable* table = table_[i].get();
    table->flush(renderer);
    renderer->BindShaderParamTable(stage(), i, table);
  }
  renderer->SetShader(stage(), shader_.get());
}

void ShaderClosure::Reset(Renderer* renderer) {
  renderer->ResetStageResource(stage());
  renderer->ResetShader(stage());
}
}  // namespace azer
