#include "azer/render/effect.h"

#include "base/logging.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/render_system.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/renderer.h"
#include "azer/render/technique.h"


namespace azer {
Effect::Effect(RenderSystem* rs)
    : Resource(kEffect)
    , render_system_(rs) {
  DCHECK(NULL != render_system_);
  gpu_table_.resize(kRenderPipelineStageNum);
}

Effect::~Effect() {
}

void Effect::Use(Renderer* renderer) {
  UseTexture(renderer);
  UseConstantsTable(renderer);
  UseTechnique(renderer);
}

void Effect::flush(Renderer* renderer) {
  DCHECK(NULL != render_system_);
  for (int i = (int)kVertexStage; i < (int)kPixelStage; ++i) {
    RenderPipelineStage stage = (RenderPipelineStage)i;
    GpuConstantsTable* table = gpu_table_[i].get();
    if (table != NULL) {
      table->flush(renderer);
    }
  }
}

void Effect::UseTechnique(Renderer* renderer) {
  DCHECK(technique_.get() != NULL);
  technique_->Use(renderer);
}

void Effect::UseConstantsTable(Renderer* renderer) {
  DCHECK(render_system_ != NULL);
  for (int i = (int)kVertexStage; i <= (int)kPixelStage; ++i) {
    GpuConstantsTable* table = gpu_table_[i].get();
    RenderPipelineStage stage = (RenderPipelineStage)i;
    if (table != NULL) {
      table->flush(renderer);
      renderer->UseConstantsTable(stage, table);
    }
  }
}
}  // namespace azer
