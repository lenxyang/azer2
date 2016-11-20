#include "azer/effect/normalline_effect.h"

#include <stddef.h>


#include "base/logging.h"

#include "azer/azer.h"

namespace azer {
IMPLEMENT_EFFECT_DYNCREATE(NormalLineEffect);
const char NormalLineEffect::kEffectName[] = "azer::NormalLineEffect";
NormalLineEffect::NormalLineEffect() : line_length_(1.0f) {}
NormalLineEffect::~NormalLineEffect() {}
const char* NormalLineEffect::GetEffectName() const { return kEffectName; }

ShaderClosurePtr NormalLineEffect::InitVertexStage(Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
  ShaderParamTablePtr table;
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
  closure->SetShader(shader, 1, 0, 0);
  closure->SetShaderParamTable(0, table.get());
  return closure;
}

ShaderClosurePtr NormalLineEffect::InitGeometryStage(Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
  ShaderParamTablePtr table;
  closure->SetShader(shader, 0
                     , 0, 0);
  return closure;
}

ShaderClosurePtr NormalLineEffect::InitPixelStage(Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
  ShaderParamTablePtr table;
  // generate GpuTable init for stage kPixelStage
  ShaderParamTable::Desc ps_table_desc[] = {
    ShaderParamTable::Desc("color", ShaderParamType::kVector4, 0, 1),
  };
  table = new ShaderParamTable(arraysize(ps_table_desc), ps_table_desc);
  closure->SetShader(shader, 1, 0, 0);
  closure->SetShaderParamTable(0, table.get());
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
