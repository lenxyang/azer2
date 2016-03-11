#include "azer/effect/normalline_effect.h"

#include <stddef.h>

#include "base/basictypes.h"
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
  gpu_table_[kVertexStage] = rs->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);


  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("color", GpuConstantsType::kVector4, 0, 1),
  };
  gpu_table_[kPixelStage] = rs->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc);
}

void NormalLineEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &line_length_, sizeof(float));
  }

  {
    GpuConstantsTable* tb = gpu_table_[(int)kPixelStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &color_, sizeof(Vector4));
  }
}
}  // namespace azer
