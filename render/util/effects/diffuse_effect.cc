#include "azer/render/util/effects/diffuse_effect.h"

#include <stddef.h>

#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/render.h"
#include "azer/render/util/effects/vertex_desc.h"

namespace azer {
const char ColoredDiffuseEffect::kEffectName[] = "ColoredDiffuseEffect";
ColoredDiffuseEffect::ColoredDiffuseEffect() {}
ColoredDiffuseEffect::~ColoredDiffuseEffect() {}

const char* ColoredDiffuseEffect::GetEffectName() const {
   return kEffectName;
}
bool ColoredDiffuseEffect::Init(VertexDesc* desc, const Shaders& sources) {
  vertex_desc_ = desc;
  DCHECK(sources.size() == kRenderPipelineStageNum);
  DCHECK(!sources[kVertexStage].code.empty());
  DCHECK(!sources[kPixelStage].code.empty());
  InitTechnique(sources);
  InitGpuConstantTable();
  return true;
}

void ColoredDiffuseEffect::InitGpuConstantTable() {
  RenderSystem* rs =  RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pvw", GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, pvw), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, world), 1),
  };
  gpu_table_[kVertexStage] = rs->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("color", GpuConstantsType::kVector4,
         offsetof(ps_cbuffer, color), 1),
    GpuConstantsTable::Desc("light", offsetof(ps_cbuffer, light),
         sizeof(DirLight), 1),
  };
  gpu_table_[kPixelStage] = rs->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc);
}
void ColoredDiffuseEffect::InitTechnique(const Shaders& sources) {
  InitShaders(sources);
}

void ColoredDiffuseEffect::ApplyGpuConstantTable(Renderer* renderer) {
  GpuConstantsTable* vs_tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(vs_tb != NULL);
  vs_tb->SetValue(0, &vs_data_.pvw, sizeof(Matrix4));
  vs_tb->SetValue(1, &vs_data_.world, sizeof(Matrix4));

  GpuConstantsTable* ps_tb = gpu_table_[(int)kPixelStage].get();
  DCHECK(ps_tb != NULL);
  ps_tb->SetValue(0, &ps_data_.color, sizeof(Vector4));
  ps_tb->SetValue(1, &ps_data_.light, sizeof(DirLight));
}

void ColoredDiffuseEffect::SetPVW(const Matrix4& value) {
  vs_data_.pvw = value;
}
void ColoredDiffuseEffect::SetWorld(const Matrix4& value) {
  vs_data_.world = value;
}
void ColoredDiffuseEffect::SetColor(const Vector4& value) {
  ps_data_.color = value;
}
void ColoredDiffuseEffect::SetDirLight(const DirLight& value) {
  ps_data_.light = value;
}

ColoredDiffuseEffectPtr CreateColoredDiffuseEffect() {
  Shaders shaders(PosNormalVertex::CreateVertexDesc());
  CHECK(LoadStageShader(kVertexStage, 
                          "azer/render/util/effects/hlsl/colored_diffuse.hlsl.vs",
                          &shaders));
  CHECK(LoadStageShader(kPixelStage, 
                          "azer/render/util/effects/hlsl/colored_diffuse.hlsl.ps",
                          &shaders));
  ColoredDiffuseEffectPtr ptr(new ColoredDiffuseEffect());
  ptr->Init(PosNormalVertex::CreateVertexDesc(), shaders);
  return ptr;
}
}  // namespace azer
