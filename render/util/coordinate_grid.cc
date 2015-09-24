#include "azer/render/util/coordinate_grid.h"

#include "azer/math/math.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "azer/render/render_system.h"

namespace azer {

// class CoordinateGrid
CoordinateGrid::CoordinateGrid(float width, float height, int num)
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
  float row_width = kHeight / (float)kNum;
  for (int i = 0; i < kNum + 1; ++i) {
    float z = kHeight / 2.0f - row_width * i;
    vertex->position = Vector4(-kWidth / 2.0f, 0.0f, z, 1.0f);
    vertex++;
    vertex->position = Vector4(kWidth / 2.0f, 0.0f, z, 1.0f);
    vertex++;
  }

  float col_width = kHeight / (float)kNum;
  for (int i = 0; i < kNum + 1; ++i) {
    float x  = kWidth / 2.0f - col_width * i;
    vertex->position = Vector4(x, 0.0f, -kHeight / 2.0f, 1.0f);
    vertex++;
    vertex->position = Vector4(x, 0.0f, kHeight / 2.0f, 1.0f);
    vertex++;
  }
  vb_ = rs->CreateVertexBuffer(VertexBuffer::Options(), data_.get());
}

void CoordinateGrid::Render(Renderer* renderer) {
  effect_ptr_->SetGridDiffuse(x_color_);
  effect_ptr_->Use(renderer);
  renderer->Draw(vb_.get(), kLineList, (kNum + 1) * 2, (kNum + 1) * 2);
  effect_ptr_->SetGridDiffuse(z_color_);
  effect_ptr_->Use(renderer);
  renderer->Draw(vb_.get(), kLineList, (kNum + 1) * 2, 0);
}


void CoordinateGrid::Update(const Camera& camera) {
  Matrix4 world = std::move(holder_.GenWorldMatrix());
  Matrix4 pvw = std::move(camera.GetProjViewMatrix() * world);
  effect_ptr_->SetPVW(pvw);
}
}  // namespace azer
