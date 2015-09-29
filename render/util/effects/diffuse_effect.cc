#include "wow/sandbox/render/mapchunk/diffuse_effect.h"

#include <stddef.h>

#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/render.h"

namespace azer {
IMPLEMENT_EFFECT_DYNCREATE(ColoredDiffuseEffect);
const char ColoredDiffuseEffect::kEffectName[] = "ColoredDiffuseEffect";
const azer::VertexDesc::Desc ColoredDiffuseEffect::kVertexDesc[] = {
  {"POSITION", 0, azer::kVec4},
  {"NORMAL", 0, azer::kVec4},
};

const int ColoredDiffuseEffect::kVertexDescNum = 
    arraysize(ColoredDiffuseEffect::kVertexDesc);

ColoredDiffuseEffect::ColoredDiffuseEffect() 
  : azer::Effect(azer::RenderSystem::Current()) {
}

ColoredDiffuseEffect::~ColoredDiffuseEffect() {
}

const char* ColoredDiffuseEffect::name() const {
   return kEffectName;
}
bool ColoredDiffuseEffect::Init(const ShaderPrograms& sources) {
  DCHECK(sources.size() == azer::kRenderPipelineStageNum);
  DCHECK(!sources[azer::kVertexStage].code.empty());
  DCHECK(!sources[azer::kPixelStage].code.empty());
  InitTechnique(sources);
  InitGpuConstantTable();
  return true;
}

void ColoredDiffuseEffect::InitGpuConstantTable() {
  // generate GpuTable init for stage azer::kVertexStage
  azer::GpuConstantsTable::Desc vs_table_desc[] = {
    azer::GpuConstantsTable::Desc("pvw", azer::GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, pvw), 1),
    azer::GpuConstantsTable::Desc("world", azer::GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, world), 1),
  };
  gpu_table_[azer::kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  // generate GpuTable init for stage azer::kPixelStage
  azer::GpuConstantsTable::Desc ps_table_desc[] = {
    azer::GpuConstantsTable::Desc("color", azer::GpuConstantsType::kVector4,
         offsetof(ps_cbuffer, color), 1),
    azer::GpuConstantsTable::Desc("light", offsetof(ps_cbuffer, light),
         sizeof(DirLight), 1),
  };
  gpu_table_[azer::kPixelStage] = render_system_->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc);
}
void ColoredDiffuseEffect::InitTechnique(const ShaderPrograms& sources) {
  vertex_desc_ptr_ = new azer::VertexDesc(kVertexDesc, kVertexDescNum);
  InitShaders(sources);
}

void ColoredDiffuseEffect::SetPVW(const azer::Matrix4& value) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &value, sizeof(azer::Matrix4));
}
void ColoredDiffuseEffect::SetWorld(const azer::Matrix4& value) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(1, &value, sizeof(azer::Matrix4));
}
void ColoredDiffuseEffect::SetColor(const azer::Vector4& value) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &value, sizeof(azer::Vector4));
}
void ColoredDiffuseEffect::SetDirLight(const DirLight& value) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(1, &value, sizeof(DirLight));
}

void ColoredDiffuseEffect::UseTexture(azer::Renderer* renderer) {
}

}  // namespace azer
