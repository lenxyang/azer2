#include "azer/effect/sky_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/render/renderer.h"


namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(SkyboxEffect);
const char SkyboxEffect::kEffectName[] = "azer::SkyboxEffect";
SkyboxEffect::SkyboxEffect() {}
SkyboxEffect::~SkyboxEffect() {}
const char* SkyboxEffect::GetEffectName() const { return kEffectName; }

void SkyboxEffect::InitGpuConstantTable() {
  RenderSystem* rs = RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, pv), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, world), 1),
  };
  gpu_table_[kVertexStage] = rs->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
}

void SkyboxEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
  }
}

void SkyboxEffect::UseTexture(Renderer* renderer) {
  renderer->BindTexture(kPixelStage, 0, cubemap_.get());
}

}  // namespace azer
