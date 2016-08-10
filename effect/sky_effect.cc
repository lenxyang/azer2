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

ShaderClosurePtr SkyboxEffect::InitShaderClosure(RenderPipelineStage stage,
                                                 Shader* shader) {
  RenderSystem* rs = RenderSystem::Current();
  ShaderParamTablePtr table;
  ShaderClosurePtr closure(new ShaderClosure(stage));
  if (stage == kVertexStage) {
    // generate GpuTable init for stage kVertexStage
    ShaderParamTable::Desc vs_table_desc[] = {
      ShaderParamTable::Desc("pv", ShaderParamType::kMatrix4,
                              offsetof(vs_cbuffer, pv), 1),
      ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                              offsetof(vs_cbuffer, world), 1),
    };
  
    table = rs->CreateShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetShaderParamTable(kVertexStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else if (stage == kPixelStage) {
    closure->SetShader(shader, 0, 0, 0);
  } else {
    CHECK(false) << "unsupport stage: " << stage;
  }
  return closure;
}

void SkyboxEffect::ApplyShaderParamTable(Renderer* renderer) {
  {
    ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
  }
}

void SkyboxEffect::SetTexture(TextureView* texture) {
  cubemap_ = texture;
  SaveShaderResource(kPixelStage, 0, texture);
}

}  // namespace azer
