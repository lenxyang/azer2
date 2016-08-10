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

ShaderClosurePtr OverlayEffect::InitShaderClosure(RenderPipelineStage stage,
                                                  Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(stage));
  if (stage == kVertexStage) {
    // generate GpuTable init for stage kVertexStage
    ShaderParamTable::Desc vs_table_desc[] = {
      ShaderParamTable::Desc("bounds", ShaderParamType::kVector4, 0, 1),
      ShaderParamTable::Desc("texbounds", ShaderParamType::kVector4,
                              sizeof(Vector4), 1),
    };

    ShaderParamTablePtr table;
    table = new ShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetShaderParamTable(kVertexStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else if (stage == kPixelStage) {
    closure->SetShader(shader, 1, 0, 0);
  } else {
    CHECK(false) << "Unsupport Stage";
  }
  return closure;
}

void OverlayEffect::ApplyShaderParamTable(Renderer* renderer) {
  {
    ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
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
