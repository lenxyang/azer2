#include "azer/effect/effect.h"

#include "base/logging.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/render_system.h"
#include "azer/render/common.h"
#include "azer/render/renderer.h"
#include "azer/render/technique.h"


namespace azer {
Effect::Effect() {}

Effect::~Effect() {}

bool Effect::Init(const TechSource& sources) {
  vertex_desc_ = sources.vertex_desc();
  DCHECK(vertex_desc_);
  technique_ = CreateTechnique(sources);
  InitGpuConstantTable();
  return true;
}
void Effect::BindTexture(int32 mode, Renderer* renderer) {}

void Effect::SetVertexDesc(VertexDesc* desc) {
  DCHECK(vertex_desc_ == NULL);
  vertex_desc_ = desc;
}

void Effect::Apply(Renderer* renderer) {
  ApplyGpuConstantTable(renderer);
  BindConstantsTable(renderer);
  DCHECK(technique_.get() != NULL);
  technique_->Use(renderer);

  BindTexture(kUpdateAll, renderer);
  FlushGpuVariables(kUpdateAll, renderer);
}

void Effect::OnRenderNewObject(Renderer* renderer) {
}

void Effect::OnRenderBegin(Renderer* renderer) {
  DCHECK(technique_.get() != NULL);
  technique_->Use(renderer);

  BindConstantsTable(renderer);
  BindTexture(kUpdateAll, renderer);
}

void Effect::OnRenderEnd() {}

void Effect::BindConstantsTable(Renderer* renderer) {
  for (auto iter = gpu_table_.begin(); iter != gpu_table_.end(); ++iter) {
    GpuConstantsTable* table = iter->table.get();
    DCHECK(table);
    renderer->BindConstantsTable((RenderPipelineStage)iter->stage, 0, table);
  }
}

void Effect::FlushGpuVariables(int type, Renderer* renderer) {
  for (auto iter = gpu_table_.begin(); iter != gpu_table_.end(); ++iter) {
    GpuConstantsTable* table = iter->table.get();
    DCHECK(table);
    table->flush(renderer);
  }
}

Technique* Effect::technique() { return technique_.get();}
void Effect::SetTechnique(Technique* technique) {technique_ = technique;}
VertexDesc* Effect::vertex_desc() { return vertex_desc_.get();}
const VertexDesc* Effect::vertex_desc() const { return vertex_desc_.get();}
}  // namespace azer
