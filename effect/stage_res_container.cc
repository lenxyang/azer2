#include "azer/effect/stage_res_container.h"

#include <algorithm>
#include "base/logging.h"
#include "azer/render/common.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/renderer.h"

namespace azer {
StageResource::StageResource() {
  stage = kStageNotSpec;
  count = 0;
  mode = 0;
  memset(tex, 0, sizeof(tex));
}

// class StageResContainer
StageResContainer::StageResContainer() {
  stages_.resize(kRenderPipelineStageNum);
  for (int32_t i = 0; i < kRenderPipelineStageNum; ++i) {
    stages_[i].stage = i;
  }
}

StageResContainer::~StageResContainer() {
}

void StageResContainer::SetResource(int stage, int index, ShaderResView* tex) {
  StageResource& s = GetStage(stage);
  s.count = std::max(s.count, index + 1);
  s.tex[index] = tex;
}

StageResource& StageResContainer::GetStage(int32_t stage) {
  DCHECK_GE(stage, 0);
  DCHECK_LT(stage, kRenderPipelineStageNum);
  return stages_[stage];
}

void StageResContainer::Bind(Renderer* renderer) {
  for (auto iter = stages_.begin(); iter != stages_.end(); ++iter) {
    if (iter->count > 0) {
      RenderPipelineStage stage = (RenderPipelineStage)iter->stage;
      renderer->SetShaderResource(stage, 0, iter->count, iter->tex);
    }
  }
}

void StageResContainer::Reset(Renderer* renderer) {
  for (auto iter = stages_.begin(); iter != stages_.end(); ++iter) {
    if (iter->count > 0) {
      RenderPipelineStage stage = (RenderPipelineStage)iter->stage;
      renderer->ResetStageResource(stage);
    }
  }
}
}  // namespace azer
