#include "azer/render/util/coordinate_grid.h"

#include "azer/math/math.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"
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

void CoordinateGridEffect::SetPVW(const azer::Matrix4& value) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, (void*)&value,  sizeof(azer::Matrix4));
}

void CoordinateGridEffect::SetGridDiffuse(const azer::Vector4& value) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, (void*)&value,  sizeof(azer::Vector4));
}

void CoordinateGridEffect::Init(RenderSystem* rs) {
  /*
  technique_ = ResourceManager::GetCommonResourceManager()->GetTechnique(
      AZER_LITERAL("technique/coordinate_grid_effect.tech"), rs);
  */
  CHECK(false);
 
 
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("vp", azer::GpuConstantsType::kMatrix4,
                            offsetof(CoordinateGridEffect::vs_cbuffer, vp), 1), 
  };
  gpu_table_[azer::kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("diffuse", azer::GpuConstantsType::kVector4,
                            offsetof(CoordinateGridEffect::ps_cbuffer, diffuse), 1), 
  };
  gpu_table_[azer::kPixelStage] = (render_system_->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc));
}

void CoordinateGridEffect::UseTexture(azer::Renderer* renderer) {
}


// class CoordinateGrid
CoordinateGrid::CoordinateGrid(int width, int height, int num)
    : x_color_(Vector4(1.0f, 0.0f, 0.0f, 1.0)),
      z_color_(Vector4(0.0f, 0.0f, 1.0f, 1.0)),
      kWidth(width), kHeight(height), kNum(num) {
  Init();
}

CoordinateGrid::~CoordinateGrid() {
}

void CoordinateGrid::Init() {
  RenderSystem* rs = RenderSystem::Current();
  std::vector<std::string> vec;
  vec.resize(kRenderPipelineStageNum);
  CoordinateGridEffect* effect = new CoordinateGridEffect(rs);
  effect_ptr_ = effect;
  data_ = new VertexData(effect->GetVertexDesc(), (kNum + 1) * 4);
  CoordinateGridEffect::Vertex* vertex =
      (CoordinateGridEffect::Vertex*)data_->pointer();
  float row_width = (float)kHeight / (float)kNum;
  for (int i = 0; i < kNum + 1; ++i) {
    float z = (float)kHeight / 2.0f - row_width * i;
    vertex->position = Vector4(-(float)kWidth / 2.0f, 0.0f, z, 1.0f);
    vertex++;
    vertex->position = Vector4((float)kWidth / 2.0f, 0.0f, z, 1.0f);
    vertex++;
  }

  float col_width = (float)kHeight / (float)kNum;
  for (int i = 0; i < kNum + 1; ++i) {
    float x  = (float)kWidth / 2.0f - col_width * i;
    vertex->position = Vector4(x, 0.0f, -(float)kHeight / 2.0f, 1.0f);
    vertex++;
    vertex->position = Vector4(x, 0.0f, (float)kHeight / 2.0f, 1.0f);
    vertex++;
  }
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), data_.get());
}

void CoordinateGrid::Render(Renderer* renderer) {
  effect_ptr_->Use(renderer);
  effect_ptr_->SetGridDiffuse(z_color_);
  renderer->Draw(vb_.get(), kLineList, (kNum + 1) * 2, 0);
  effect_ptr_->SetGridDiffuse(x_color_);
  renderer->Draw(vb_.get(), kLineList, (kNum + 1) * 2, (kNum + 1) * 2);
}


void CoordinateGrid::SetPVWMat(const Matrix4& mat) {
  effect_ptr_->SetPVW(mat);
}
}  // namespace azer
