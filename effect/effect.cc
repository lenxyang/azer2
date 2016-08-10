#include "azer/effect/effect.h"

#include "base/logging.h"
#include "azer/effect/shader_closure.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/render_system.h"
#include "azer/render/common.h"
#include "azer/render/renderer.h"
#include "azer/render/technique.h"

namespace azer {
Effect::Effect() {
  shaders_.resize((int)kRenderPipelineStageNum);
}

Effect::~Effect() {}

bool Effect::Init(const TechSource& sources) {
  vertex_desc_ = sources.vertex_desc();
  DCHECK(vertex_desc_);
  technique_ = CreateTechnique(sources);
  shaders_.resize(kRenderPipelineStageNum);
  for (int i = 0; i < (int)kRenderPipelineStageNum; ++i) {
    Shader* shader = technique_->GetShader(i);
    if (shader != NULL) {
      ShaderClosurePtr closure = InitShaderClosure(i, shader);
      shaders_[i] = closure;
    }
  }
  return true;
}

void Effect::SetVertexDesc(VertexDesc* desc) {
  DCHECK(vertex_desc_ == NULL);
  vertex_desc_ = desc;
}

ShaderClosure* Effect::GetShaderClosure(int stage) {
  return shaders_[stage].get();
}

void Effect::ApplyGpuConstantTable(Renderer* renderer) {
}

ShaderClosurePtr Effect::InitShaderClosure(int stage, Shader* shader) {
  CHECK(false);
  return ShaderClosurePtr();
}

void Effect::SetShaderClosure(ShaderClosure* closure) {
  shaders_[(int)closure->stage()] = closure;
}

void Effect::Apply(Renderer* renderer) {
  ApplyGpuConstantTable(renderer);
  // BindConstantsTable(renderer);
  // technique_->Bind(renderer);
  DCHECK(technique_.get() != NULL);

  FlushGpuVariables(0, renderer);
}

void Effect::SaveShaderResource(int stage, int index, ShaderResView* tex) {
  CHECK(shaders_[stage].get());
  shaders_[stage]->SetResource(index, 1, tex);
}

void Effect::SetGpuConstantsTable(int stage, int index, GpuConstantsTable* table) {
  CHECK(shaders_[stage].get());
  shaders_[stage]->SetGpuConstantsTable(stage, index, table);
}

void Effect::OnRenderNewObject(Renderer* renderer) {
}

void Effect::OnRenderBegin(Renderer* renderer) {
  DCHECK(technique_.get() != NULL);
  // technique_->Bind(renderer);
  // BindConstantsTable(renderer);
  // shaders_[stage]->Bind(renderer);
  for (auto iter = shaders_.begin(); iter != shaders_.end(); ++iter) {
    if (!iter->get()) continue;
    (*iter)->Bind(renderer);
  }
}

void Effect::OnRenderEnd(Renderer* renderer) {
  for (auto iter = shaders_.begin(); iter != shaders_.end(); ++iter) {
    if (!iter->get()) continue;
    (*iter)->Reset(renderer);
  }
}

void Effect::FlushGpuVariables(int type, Renderer* renderer) {
  for (auto iter = shaders_.begin(); iter != shaders_.end(); ++iter) {
    if (!iter->get()) continue;
    (*iter)->UpdateShaderParams(renderer);
  }
}

Technique* Effect::technique() { return technique_.get();}
void Effect::SetTechnique(Technique* technique) {technique_ = technique;}
VertexDesc* Effect::vertex_desc() { return vertex_desc_.get();}
const VertexDesc* Effect::vertex_desc() const { return vertex_desc_.get();}
}  // namespace azer
