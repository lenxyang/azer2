#include "azer/util/interactive/gridline.h"

#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"

namespace azer {

Gridline::Gridline(int32_t row, int32_t column, float cell) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  int32_t kVertexCount = (row + 1) * (column + 1) * 2;
  effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");

  VertexDataPtr vdata(new VertexData(effect_->vertex_desc(), kVertexCount));
  EntityDataPtr data(new EntityData(vdata));
  VertexPack vpack(vdata);
  vpack.first();
  
  float depth = row * cell;
  float width = column * cell;
  float row_begin = -row * cell * 0.5f;
  float column_begin = -column * cell * 0.5f;
  VertexPos ppos(0, 0);
  for (int32_t i = 0; i < row + 1; ++i) {
    float x = row_begin + i * cell;
    vpack.WriteVector3(Vector3(x, 0.0f, column_begin), ppos);
    vpack.next(1);
    vpack.WriteVector3(Vector3(x, 0.0f, column_begin + depth), ppos);
    vpack.next(1);
  }

  for (int32_t i = 0; i < column + 1; ++i) {
    float z = column_begin + i * cell;
    vpack.WriteVector3(Vector3(row_begin, 0.0f, z), ppos);
    vpack.next(1);
    vpack.WriteVector3(Vector3(row_begin + width, 0.0f, z), ppos);
    vpack.next(1);
  }

  Subset subset;
  subset.vertex_base = 0;
  subset.vertex_count = kVertexCount;
  subset.primitive = kLineList;
  data->AddSubset(subset);
  entity_ = new Entity(data);
  color_ = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
}

Gridline::~Gridline() {
}
  
void Gridline::Update(const Camera& camera) {
  world_ = Translate(position_);
  pv_ = camera.GetProjViewMatrix();
}

void Gridline::Render(Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_->SetWorld(world_);
  effect_->SetPV(pv_);
  effect_->SetAmbient(color_);
  renderer->BindEffect(effect_);
  entity_->Draw(renderer);
}
}  // namespace azer
