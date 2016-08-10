#include "azer/effect/normalline_effect.h"

#include <stddef.h>


#include "base/logging.h"

#include "azer/azer.h"
#include "azer/scene/render_node.h"
#include "azer/scene/scene_node.h"

namespace azer {
IMPLEMENT_EFFECT_DYNCREATE(NormalLineEffect);
const char NormalLineEffect::kEffectName[] = "azer::NormalLineEffect";
NormalLineEffect::NormalLineEffect() : line_length_(1.0f) {}
NormalLineEffect::~NormalLineEffect() {}
const char* NormalLineEffect::GetEffectName() const { return kEffectName; }

ShaderClosurePtr NormalLineEffect::InitShaderClosure(RenderPipelineStage stage,
                                                     Shader* shader) {
  ShaderParamTablePtr table;
  ShaderClosurePtr closure(new ShaderClosure(stage));
  if (stage == kVertexStage) {
    // generate GpuTable init for stage kVertexStage
    ShaderParamTable::Desc vs_table_desc[] = {
      ShaderParamTable::Desc("pv", ShaderParamType::kMatrix4,
                              offsetof(vs_cbuffer, pv), 1),
      ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                              offsetof(vs_cbuffer, world), 1),
      ShaderParamTable::Desc("linelength", ShaderParamType::kFloat,
                              offsetof(vs_cbuffer, linelength), 1),
    };
  
    table = new ShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetShaderParamTable(kVertexStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else if (stage == kPixelStage) {

    // generate GpuTable init for stage kPixelStage
    ShaderParamTable::Desc ps_table_desc[] = {
      ShaderParamTable::Desc("color", ShaderParamType::kVector4, 0, 1),
    };
    table = new ShaderParamTable(arraysize(ps_table_desc), ps_table_desc);
    closure->SetShaderParamTable(kPixelStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else {
    CHECK(false);
  }
  return closure;
}

void NormalLineEffect::ApplyShaderParamTable(Renderer* renderer) {
  {
    ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &line_length_, sizeof(float));
  }

  {
    ShaderParamTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &color_, sizeof(Vector4));
  }
}
}  // namespace azer
