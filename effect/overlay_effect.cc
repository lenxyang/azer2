#include "azer/effect/overlay_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/azer.h"


namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(OverlayEffect);
const char OverlayEffect::kEffectName[] = "azer::OverlayEffect";
OverlayEffect::OverlayEffect() {}
OverlayEffect::~OverlayEffect() {}
const char* OverlayEffect::GetEffectName() const { return kEffectName;}

void OverlayEffect::InitGpuConstantTable() {
  RenderSystem* rs = RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("bounds", GpuConstantsType::kVector4, 0, 1),
    GpuConstantsTable::Desc("texbounds", GpuConstantsType::kVector4,
                            sizeof(Vector4), 1),
  };

  GpuVariable v;
  v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  v.stage = kVertexStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
}

void OverlayEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuVariable gv = gpu_table_[0];
    CHECK_EQ(gv.stage, kVertexStage);
    GpuConstantsTable* tb = gv.table;
    DCHECK(tb != NULL);
    tb->SetValue(0, &bounds_, sizeof(Vector4));
    tb->SetValue(1, &texbounds_, sizeof(Vector4));
  }
}

void OverlayEffect::SetTexture(Texture* texture) {
  texture_ = texture;
  SaveShaderResTexture(kPixelStage, 0, texture_);
}
}  // namespace azer
