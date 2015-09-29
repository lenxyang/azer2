#include "wow/sandbox/render/mapchunk/diffuse_effect.h"

#include <stddef.h>

#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/render.h"

namespace azer {
IMPLEMENT_EFFECT_DYNCREATE(ColoredDiffuseEffect);
const char ColoredDiffuseEffect::kEffectName[] = "ColoredDiffuseEffect";
const VertexDesc::Desc ColoredDiffuseEffect::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
  {"NORMAL", 0, kVec4},
};

const int ColoredDiffuseEffect::kVertexDescNum = 
    arraysize(ColoredDiffuseEffect::kVertexDesc);

ColoredDiffuseEffect::ColoredDiffuseEffect() 
  : Effect(RenderSystem::Current()) {
}

ColoredDiffuseEffect::~ColoredDiffuseEffect() {
}

const char* ColoredDiffuseEffect::name() const {
   return kEffectName;
}
bool ColoredDiffuseEffect::Init(const ShaderPrograms& sources) {
  DCHECK(sources.size() == kRenderPipelineStageNum);
  DCHECK(!sources[kVertexStage].code.empty());
  DCHECK(!sources[kPixelStage].code.empty());
  InitTechnique(sources);
  InitGpuConstantTable();
  return true;
}

void ColoredDiffuseEffect::InitGpuConstantTable() {
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pvw", GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, pvw), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, world), 1),
  };
  gpu_table_[kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("color", GpuConstantsType::kVector4,
         offsetof(ps_cbuffer, color), 1),
    GpuConstantsTable::Desc("light", offsetof(ps_cbuffer, light),
         sizeof(DirLight), 1),
  };
  gpu_table_[kPixelStage] = render_system_->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc);
}
void ColoredDiffuseEffect::InitTechnique(const ShaderPrograms& sources) {
  vertex_desc_ptr_ = new VertexDesc(kVertexDesc, kVertexDescNum);
  InitShaders(sources);
}

void ColoredDiffuseEffect::SetPVW(const Matrix4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &value, sizeof(Matrix4));
}
void ColoredDiffuseEffect::SetWorld(const Matrix4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(1, &value, sizeof(Matrix4));
}
void ColoredDiffuseEffect::SetColor(const Vector4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kPixelStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &value, sizeof(Vector4));
}
void ColoredDiffuseEffect::SetDirLight(const DirLight& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kPixelStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(1, &value, sizeof(DirLight));
}

void ColoredDiffuseEffect::UseTexture(Renderer* renderer) {
}

ColoredDiffuseEffectPtr ColoredDiffuseEffect() {
  Effect::ShaderPrograms shaders;
  CHECK(LoadShaderAtStage(kVertexStage, 
                          "azer/render/util/effects/hlsl/colored_diffuse.hlsl.vs",
                          &shaders));
  CHECK(LoadShaderAtStage(kPixelStage, 
                          "azer/render/util/effects/hlsl/colored_diffuse.hlsl.ps",
                          &shaders));
  PVWEffectPtr ptr(new PVWEffect(PositionVertex::PosNormalVertex()));
  ptr->Init(shaders);
  return ptr;
}
}  // namespace azer
