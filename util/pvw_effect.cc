#include "wow/render/pvw_effect.h"

#include <stddef.h>

#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/render.h"
#include "wow/render/vertex_desc.h"
#include "wow/render/util.h"

namespace azer {
const char PVWEffect::kEffectName[] = "PVWEffect";

PVWEffect::PVWEffect(VertexDescPtr desc) 
    : Effect(RenderSystem::Current()) {
  vertex_desc_ptr_ = desc;
}

PVWEffect::~PVWEffect() {
}

const char* PVWEffect::name() const {
   return kEffectName;
}
bool PVWEffect::Init(const ShaderPrograms& sources) {
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
  gpu_table_[azer::kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  return true;
}

void PVWEffect::SetPVW(const Matrix4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &value, sizeof(Matrix4));
}
void PVWEffect::SetWorld(const Matrix4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(1, &value, sizeof(Matrix4));
}

void PVWEffect::UseTexture(Renderer* renderer) {
}


PVWEffectPtr CreateDefaultPVWEffect() {
  Effect::ShaderPrograms shaders;
  CHECK(wow::LoadShaderAtStage(azer::kVertexStage,
                               "wow/render/hlsl/default_pvw.hlsl.vs",
                               &shaders));
  CHECK(wow::LoadShaderAtStage(azer::kPixelStage,
                               "wow/render/hlsl/default_pvw.hlsl.ps",
                               &shaders));
  PVWEffectPtr ptr(new PVWEffect(azer::PositionVertex::CreateVertexDesc()));
  ptr->Init(shaders);
  return ptr;
}
}  // namespace azer
