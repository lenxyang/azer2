#include "azer/effect/stage_texture.h"

#include "base/common/logging.h"
#include "azer/render/common.h"
#include "azer/render/renderer.h"

namespace azer {
StageTexture::StageTexture() {
  stage = kStageNotSpec;
  count = 0;
  mode = 0;
  memset(tex, 0, sizeof(tex));
}

// class StageTexContainer
StageTexContainer::StageTexContainer() {
  stages_.resize(kRenderPipelineStageNum);
  for (int32 i = 0; i < kRenderPipelineStageNum; ++i) {
    stages_[i].stage = i;
  }
}

StageTexContainer::~StageTexContainer() {
}

void StageTexContainer::SetTex(int stage, int index, Texture* tex) {
  StageTexture& stage = GetStage(stage);
  stage.count = std::max(stage.count, index + 1);
  stage.tex[index] = tex;
}

StageTexture& StageTexContainer::GetStage(int32 stage) {
  DCHECK_GE(stage, 0);
  DCHECK_LT(stage, kRenderPipelineStageNum);
  return stages_[stage];
}

void StageTexContainer::Bind(Renderer* renderer) {
  for (auto iter = stages_.begin(); iter != stages_.end(); ++iter) {
    if (iter->count > 0) {
      renderer->BindTexture(stage, 0, iter->count, iter->tex);
    }
  }
}

void StageTexCounter::Reset(Renderer* renderer) {
  for (auto iter = stages_.begin(); iter != stages_.end(); ++iter) {
    if (iter->count > 0) {
      renderer->ResetStageTexture(stage);
    }
  }
}
}  // namespace azer
