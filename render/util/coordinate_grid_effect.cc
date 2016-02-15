#include "azer/render/util/coordinate_grid_effect.h"

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/render_system_loader.h"

namespace azer {
const char CoordinateGridEffect::kEffectName[] = "CoordinateGridEffect";
// class CoordinateGridEffect
const VertexDesc::Desc CoordinateGridEffect::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
};
const int CoordinateGridEffect::kVertexDescNum =
    arraysize(CoordinateGridEffect::kVertexDesc);

CoordinateGridEffect::CoordinateGridEffect() {
  InitEffect();
}

const char* CoordinateGridEffect::GetEffectName() const {
  return kEffectName;
}

void CoordinateGridEffect::ApplyGpuConstantTable(Renderer* renderer) {
  GpuConstantsTable* vstb = gpu_table_[(int)kVertexStage].get();
  DCHECK(vstb != NULL);
  vstb->SetValue(0, (void*)&vs_data_.vp,  sizeof(Matrix4));

  GpuConstantsTable* pstb = gpu_table_[(int)kPixelStage].get();
  DCHECK(pstb != NULL);
  pstb->SetValue(0, (void*)&ps_data_.diffuse,  sizeof(Vector4));
}

void CoordinateGridEffect::SetPVW(const Matrix4& value) {
  vs_data_.vp = value;
}

void CoordinateGridEffect::SetGridDiffuse(const Vector4& value) {
  ps_data_.diffuse = value;
  
}

void CoordinateGridEffect::InitEffect() {
  RenderSystem* rs = RenderSystem::Current();
  vertex_desc_ = new azer::VertexDesc(kVertexDesc, kVertexDescNum);
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("vp", GpuConstantsType::kMatrix4,
                            offsetof(CoordinateGridEffect::vs_cbuffer, vp), 1), 
  };
  gpu_table_[kVertexStage] = rs->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("diffuse", GpuConstantsType::kVector4,
                            offsetof(CoordinateGridEffect::ps_cbuffer, diffuse), 1), 
  };
  gpu_table_[kPixelStage] = (rs->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc));

  ShaderInfo shader;
  TechSource sources(vertex_desc_);
  shader.code = GetCommonShaderProgram(kEffectName, "", kVertexStage, NULL);
  shader.path = kEffectName;
  sources.SetStage(kVertexStage, shader);
  
  shader.code = GetCommonShaderProgram(kEffectName, "", kPixelStage, NULL);
  shader.path = kEffectName;
  sources.SetStage(kPixelStage, shader);
  InitShaders(sources);
}

}  // namespace azer
