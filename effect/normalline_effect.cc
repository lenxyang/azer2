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

void NormalLineEffect::InitGpuConstantTable() {
  RenderSystem* rs = RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, pv), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, world), 1),
    GpuConstantsTable::Desc("linelength", GpuConstantsType::kFloat,
                            offsetof(vs_cbuffer, linelength), 1),
  };
  GpuVariable v;
  v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  v.stage = kVertexStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);


  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("color", GpuConstantsType::kVector4, 0, 1),
  };
  v.table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
  v.stage = kPixelStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
}

void NormalLineEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuVariable gv = gpu_table_[0];
    CHECK_EQ(gv.stage, kVertexStage);
    GpuConstantsTable* tb = gv.table.get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &line_length_, sizeof(float));
  }

  {
    GpuVariable gv = gpu_table_[1];
    CHECK_EQ(gv.stage, kPixelStage);
    GpuConstantsTable* tb = gv.table.get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &color_, sizeof(Vector4));
  }
}
}  // namespace azer
