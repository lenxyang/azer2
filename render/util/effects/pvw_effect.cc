#include "azer/render/util/effects/pvw_effect.h"

#include <stddef.h>

#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/render.h"
#include "azer/render/util/shader_util.h"
#include "azer/render/util/effects/vertex_desc.h"

namespace azer {
const char PVWEffect::kEffectName[] = "PVWEffect";

PVWEffect::PVWEffect() {}
PVWEffect::~PVWEffect() {}

const char* PVWEffect::GetEffectName() const {
  return kEffectName;
}
bool PVWEffect::Init(VertexDesc* desc, const ShaderPrograms& sources) {
  vertex_desc_ = desc;
  RenderSystem* rs = RenderSystem::Current();
  DCHECK(sources.size() == kRenderPipelineStageNum);
  DCHECK(!sources[kVertexStage].code.empty());
  DCHECK(!sources[kPixelStage].code.empty());

  InitShaders(sources);

  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pvw", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, pvw), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, world), 1),
  };
  gpu_table_[kVertexStage] = rs->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  return true;
}

void PVWEffect::SetPVW(const Matrix4& value) {
  data_.pvw = value;
}
void PVWEffect::SetWorld(const Matrix4& value) {
  data_.world = value;
}

void PVWEffect::ApplyGpuConstantTable(Renderer* renderer) {
  GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &data_.pvw, sizeof(Matrix4));
  tb->SetValue(1, &data_.world, sizeof(Matrix4));
}

PVWEffectPtr CreatePVWEffect() {
  Effect::ShaderPrograms shaders;
  CHECK(LoadShaderAtStage(kVertexStage,
                          "azer/render/util/effects/hlsl/pvw.hlsl.vs",
                          &shaders));
  CHECK(LoadShaderAtStage(kPixelStage,
                          "azer/render/util/effects/hlsl/pvw.hlsl.ps",
                          &shaders));
  PVWEffectPtr ptr(new PVWEffect);
  ptr->Init(PositionVertex::CreateVertexDesc(), shaders);
  return ptr;
}
}  // namespace azer
