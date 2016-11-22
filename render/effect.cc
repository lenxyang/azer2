#include "azer/render/effect.h"

#include "base/logging.h"
#include "azer/render/shader_closure.h"
#include "azer/render/shader_param_table.h"
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
  OnBeforeShaderClosureInit();
  for (int i = 0; i < (int)kRenderPipelineStageNum; ++i) {
    Shader* shader = technique_->GetShader(i);
    if (shader != NULL) {
      RenderPipelineStage stage = (RenderPipelineStage)i;
      ShaderClosurePtr closure = InitShaderClosure(stage, shader);
      shaders_[i] = closure;
    }
  }
  OnAfterShaderClosureInit();
  return true;
}

void Effect::SetVertexDesc(VertexDesc* desc) {
  DCHECK(vertex_desc_ == NULL);
  vertex_desc_ = desc;
}

ShaderClosure* Effect::GetShaderClosure(int stage) {
  return shaders_[stage].get();
}

void Effect::ApplyShaderParamTable(Renderer* renderer) {
}

ShaderClosurePtr Effect::InitShaderClosure(RenderPipelineStage stage, 
                                           Shader* shader) {
  ShaderClosurePtr shader_closure;
  switch (stage) {
    case kVertexStage: 
      shader_closure = InitVertexStage(shader);
      break;
    case kHullStage: 
      shader_closure = InitHullStage(shader);
      break;
    case kDomainStage: 
      shader_closure = InitDomainStage(shader);
      break;
    case kGeometryStage: 
      shader_closure = InitGeometryStage(shader);
      break;
    case kPixelStage: 
      shader_closure = InitPixelStage(shader);
      break;
    case kComputeStage: 
      shader_closure = InitComputeStage(shader);
      break;
    default: CHECK(false) << "Not Invalid stage: " << stage;
      return ShaderClosurePtr();
  }
  return shader_closure;
}

ShaderClosurePtr Effect::InitVertexStage(Shader* shader) {
  CHECK(false) << "Unsupport stage: " << kVertexStage;
  return ShaderClosurePtr();
}

ShaderClosurePtr Effect::InitHullStage(Shader* shader) {
  CHECK(false) << "Unsupport stage: " << kHullStage;
  return ShaderClosurePtr();
}

ShaderClosurePtr Effect::InitDomainStage(Shader* shader) {
  CHECK(false) << "Unsupport stage: " << kDomainStage;
  return ShaderClosurePtr();
}

ShaderClosurePtr Effect::InitGeometryStage(Shader* shader) {
  CHECK(false) << "Unsupport stage: " << kGeometryStage;
  return ShaderClosurePtr();
}

ShaderClosurePtr Effect::InitPixelStage(Shader* shader) {
  CHECK(false) << "Unsupport stage: " << kPixelStage;
  return ShaderClosurePtr();
}

ShaderClosurePtr Effect::InitComputeStage(Shader* shader) {
  CHECK(false) << "Unsupport stage: " << kComputeStage;
  return ShaderClosurePtr();
}

void Effect::SetShaderClosure(ShaderClosure* closure) {
  shaders_[(int)closure->stage()] = closure;
}

bool IsProgramableStage(int stage) {
  switch (stage) {
    case kVertexStage: 
    case kHullStage:
    case kDomainStage:
    case kComputeStage: 
    case kGeometryStage:
    case kPixelStage:
      return true;
    default:
      return false;
  }
}

void Effect::Apply(Renderer* renderer) {
  ApplyShaderParamTable(renderer);
  for (int i = 0; i < (int)kRenderPipelineStageNum; ++i) {
    if (shaders_[i].get()) {
      shaders_[i]->Bind(renderer);
    } else if (IsProgramableStage(i)) {
      renderer->SetShader(i, NULL);
    }
  }
}

void Effect::SaveShaderResource(int stage, int index, ShaderResView* tex) {
  CHECK(shaders_[stage].get());
  shaders_[stage]->SetResource(index, tex);
}

void Effect::SetShaderParamTable(int stage, int index, ShaderParamTable* table) {
  CHECK(shaders_[stage].get());
  shaders_[stage]->SetShaderParamTable(index, table);
}

void Effect::OnRenderNewObject(Renderer* renderer) {
}

void Effect::OnRenderBegin(Renderer* renderer) {
  DCHECK(technique_.get() != NULL);
  // technique_->Bind(renderer);
  // BindShaderParamTable(renderer);
  // shaders_[stage]->Bind(renderer);
  for (auto iter = shaders_.begin(); iter != shaders_.end(); ++iter) {
    if (iter->get()) {
      (*iter)->Bind(renderer);
    }
  }
}

void Effect::OnRenderEnd(Renderer* renderer) {
  for (auto iter = shaders_.begin(); iter != shaders_.end(); ++iter) {
    if (!iter->get()) continue;
    (*iter)->Reset(renderer);
  }
}

void Effect::UpdateGpuParams(int type, Renderer* renderer) {
  for (auto iter = shaders_.begin(); iter != shaders_.end(); ++iter) {
    if (iter->get()) 
      (*iter)->UpdateShaderParam(renderer);
  }
}

void Effect::OnAfterShaderClosureInit() {
}

void Effect::OnBeforeShaderClosureInit() {
}

Technique* Effect::technique() { return technique_.get();}
void Effect::SetTechnique(Technique* technique) {technique_ = technique;}
VertexDesc* Effect::vertex_desc() { return vertex_desc_.get();}
const VertexDesc* Effect::vertex_desc() const { return vertex_desc_.get();}
}  // namespace azer
