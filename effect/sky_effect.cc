#include "azer/effect/sky_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"


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
  GpuVariable v;
  v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  v.stage = kVertexStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
}

void SkyboxEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = gpu_table_[0].table;
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
  }
}

void SkyboxEffect::SetTexture(Texture* texture) {
  cubemap_ = texture;
  SaveShaderResTexture(kPixelStage, 0, cubemap_);
}

}  // namespace azer
