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

void TextBillboardEffect::InitGpuConstantTable() {
  RenderSystem* rs = RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, world), 1),
  };
  GpuVariable v;
  v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  v.stage = kVertexStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);

  GpuConstantsTable::Desc gs_table_desc[] = {
    GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                            offsetof(gs_cbuffer, pv), 1),
    GpuConstantsTable::Desc("viewup", GpuConstantsType::kVector4,
                            offsetof(gs_cbuffer, viewup), 1),
    GpuConstantsTable::Desc("viewpos", GpuConstantsType::kVector4,
                            offsetof(gs_cbuffer, viewpos), 1),
    GpuConstantsTable::Desc("param", GpuConstantsType::kVector4,
                            offsetof(gs_cbuffer, param), 1),
  };
  v.table = rs->CreateGpuConstantsTable(arraysize(gs_table_desc), gs_table_desc);
  v.stage = kGeometryStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);

  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("diffuse", GpuConstantsType::kVector4,
                            offsetof(ps_cbuffer, diffuse), 1),
  };
  v.table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
  v.stage = kPixelStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
}

void TextBillboardEffect::SetBillboard(float width, float height) {
  param_.x = width;
  param_.y = height;
}

void TextBillboardEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuVariable gv = gpu_table_[0];
    CHECK_EQ(gv.stage, kVertexStage);
    GpuConstantsTable* tb = gv.table;
    DCHECK(tb != NULL);
    tb->SetValue(0, &world_, sizeof(Matrix4));
  }
  {
    GpuVariable gv = gpu_table_[1];
    CHECK_EQ(gv.stage, kGeometryStage);
    GpuConstantsTable* tb = gv.table;
    tb->SetValue(0, &pv_, sizeof(pv_));
    tb->SetValue(1, &viewup_, sizeof(viewup_));
    tb->SetValue(2, &viewpos_, sizeof(viewpos_));
    tb->SetValue(3, &param_, sizeof(param_));
  }
  {
    GpuVariable gv = gpu_table_[2];
    CHECK_EQ(gv.stage, kPixelStage);
    GpuConstantsTable* tb = gv.table;
    DCHECK(tb != NULL);
    tb->SetValue(0, &diffuse_, sizeof(diffuse_));
  }
}

void TextBillboardEffect::BindTexture(int32 mode, Renderer* renderer) {
  renderer->BindTexture(kPixelStage, 0, tex_.get());
}
}  // namespace azer
