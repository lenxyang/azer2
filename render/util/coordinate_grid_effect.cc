#include "azer/render/util/coordinate_grid_effect.h"

#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {
const char CoordinateGridEffect::kEffectName[] = "CoordinateGridEffect";
// class CoordinateGridEffect
const VertexDesc::Desc CoordinateGridEffect::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
};
const int CoordinateGridEffect::kVertexDescNum =
    arraysize(CoordinateGridEffect::kVertexDesc);

CoordinateGridEffect::CoordinateGridEffect(RenderSystem* rs)
    : Effect(rs) {
  Init(rs);
}

const char* CoordinateGridEffect::name() const {
  return kEffectName;
}

void CoordinateGridEffect::SetPVW(const Matrix4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, (void*)&value,  sizeof(Matrix4));
}

void CoordinateGridEffect::SetGridDiffuse(const Vector4& value) {
  GpuConstantsTable* tb = gpu_table_[(int)kPixelStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, (void*)&value,  sizeof(Vector4));
}

void CoordinateGridEffect::Init(RenderSystem* rs) {
  vertex_desc_ptr_ = new azer::VertexDesc(kVertexDesc, kVertexDescNum);
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("vp", GpuConstantsType::kMatrix4,
                            offsetof(CoordinateGridEffect::vs_cbuffer, vp), 1), 
  };
  gpu_table_[kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("diffuse", GpuConstantsType::kVector4,
                            offsetof(CoordinateGridEffect::ps_cbuffer, diffuse), 1), 
  };
  gpu_table_[kPixelStage] = (render_system_->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc));

  InitShader();
}

void CoordinateGridEffect::UseTexture(Renderer* renderer) {
}

}  // namespace azer
