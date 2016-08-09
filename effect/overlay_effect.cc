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

  GpuConstantsTablePtr table;
  table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  SetGpuConstantsTable(kVertexStage, 0, table.get());
}

void OverlayEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = GetShaderClosure(kVertexStage)->table(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &bounds_, sizeof(Vector4));
    tb->SetValue(1, &texbounds_, sizeof(Vector4));
  }
}


void OverlayEffect::SetTexture(TextureView* texture) {
  texture_ = texture;
  SaveShaderResource(kPixelStage, 0, texture);
}
}  // namespace azer
