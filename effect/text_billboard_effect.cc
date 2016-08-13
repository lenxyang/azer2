#include "azer/effect/text_billboard_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/azer.h"


namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(TextBillboardEffect);
const char TextBillboardEffect::kEffectName[] = "azer::TextBillboardEffect";
TextBillboardEffect::TextBillboardEffect() {}
TextBillboardEffect::~TextBillboardEffect() {}
const char* TextBillboardEffect::GetEffectName() const { return kEffectName;}

ShaderClosurePtr TextBillboardEffect::InitVertexStage(Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
  ShaderParamTablePtr table;
  // generate GpuTable init for stage kVertexStage
  ShaderParamTable::Desc vs_table_desc[] = {
    ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                           offsetof(vs_cbuffer, world), 1),
  };
  
  table = new ShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
  closure->SetShader(shader, 1, 0, 0);
  closure->SetShaderParamTable(0, table.get());
  return closure;
}

ShaderClosurePtr TextBillboardEffect::InitPixelStage(Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
  ShaderParamTablePtr table;
  // generate GpuTable init for stage kPixelStage
  ShaderParamTable::Desc ps_table_desc[] = {
    ShaderParamTable::Desc("diffuse", ShaderParamType::kVector4,
                           offsetof(ps_cbuffer, diffuse), 1),
  };
  table = new ShaderParamTable(arraysize(ps_table_desc), ps_table_desc);
  closure->SetShader(shader, 1, 1, 0);
  closure->SetShaderParamTable(0, table.get());

  return closure;
}

ShaderClosurePtr TextBillboardEffect::InitGeometryStage(Shader* shader) {
  ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
  ShaderParamTablePtr table;
  ShaderParamTable::Desc gs_table_desc[] = {
    ShaderParamTable::Desc("pv", ShaderParamType::kMatrix4,
                           offsetof(gs_cbuffer, pv), 1),
    ShaderParamTable::Desc("viewup", ShaderParamType::kVector4,
                           offsetof(gs_cbuffer, viewup), 1),
    ShaderParamTable::Desc("viewpos", ShaderParamType::kVector4,
                           offsetof(gs_cbuffer, viewpos), 1),
    ShaderParamTable::Desc("param", ShaderParamType::kVector4,
                           offsetof(gs_cbuffer, param), 1),
  };
  table = new ShaderParamTable(arraysize(gs_table_desc), gs_table_desc);
  closure->SetShader(shader, 1, 0, 0);
  closure->SetShaderParamTable(0, table.get());
  return closure;
}

void TextBillboardEffect::SetBillboard(float width, float height) {
  param_.x = width;
  param_.y = height;
}

void TextBillboardEffect::ApplyShaderParamTable(Renderer* renderer) {
  {
    ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &world_, sizeof(Matrix4));
  }
  {
    ShaderParamTable* tb = GetShaderClosure(kGeometryStage)->table_at(0);
    tb->SetValue(0, &pv_, sizeof(pv_));
    tb->SetValue(1, &viewup_, sizeof(viewup_));
    tb->SetValue(2, &viewpos_, sizeof(viewpos_));
    tb->SetValue(3, &param_, sizeof(param_));
  }
  {
    ShaderParamTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &diffuse_, sizeof(diffuse_));
  }
}

void TextBillboardEffect::SetTexture(TextureView* tex) {
  tex_ = tex;
  SaveShaderResource(kPixelStage, 0, tex);
}
}  // namespace azer
