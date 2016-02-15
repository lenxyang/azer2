#include "azer/render/effect.h"

#include "base/logging.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/render_system.h"
#include "azer/render/common.h"
#include "azer/render/renderer.h"
#include "azer/render/technique.h"


namespace azer {
Effect::Effect()
    : Resource(kEffect) {
  gpu_table_.resize(kRenderPipelineStageNum);
}

Effect::~Effect() {
}

void Effect::UseTexture(Renderer* renderer) {
}

void Effect::SetVertexDesc(VertexDesc* desc) {
  DCHECK(vertex_desc_ == NULL);
  vertex_desc_ = desc;
}

void Effect::Apply(Renderer* renderer) {
  UseTexture(renderer);
  ApplyGpuConstantTable(renderer);
  BindConstantsTable(renderer);
  BindTechnique(renderer);
  
}

void Effect::flush(Renderer* renderer) {
  for (int i = (int)kVertexStage; i < (int)kPixelStage; ++i) {
    RenderPipelineStage stage = (RenderPipelineStage)i;
    GpuConstantsTable* table = gpu_table_[i].get();
    if (table != NULL) {
      table->flush(renderer);
    }
  }
}

void Effect::BindTechnique(Renderer* renderer) {
  DCHECK(technique_.get() != NULL);
  technique_->Use(renderer);
}

void Effect::BindConstantsTable(Renderer* renderer) {
  for (int i = (int)kVertexStage; i <= (int)kPixelStage; ++i) {
    GpuConstantsTable* table = gpu_table_[i].get();
    RenderPipelineStage stage = (RenderPipelineStage)i;
    if (table != NULL) {
      table->flush(renderer);
      renderer->BindConstantsTable(stage, table);
    }
  }
}

void Effect::InitShaders(const TechSource& sources) {
  CHECK(vertex_desc_.get());
  RenderSystem* rs = RenderSystem::Current();
  auto vs_shader_src = sources[kVertexStage];
  ShaderPtr vs_gpup_ptr(rs->CreateVertexShader(vertex_desc_, vs_shader_src));
  CHECK(vs_gpup_ptr.get() != NULL);
  CHECK(!technique_.get());
  technique_ = rs->CreateTechnique();
  technique_->AddShader(vs_gpup_ptr);
  for (uint32 i = (uint32)(kVertexStage + 1); i < sources.size(); ++i) {
    auto info = sources[i];
    if (!info.code.empty()) {
      ShaderPtr gpup_ptr(rs->CreateShader(info));
      CHECK(gpup_ptr.get() != NULL);
      technique_->AddShader(gpup_ptr);
    }
  }
}

}  // namespace azer
