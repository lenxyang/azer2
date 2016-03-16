#include "azer/util/interactive/axes_frame.h"

#include "base/logging.h"
#include "azer/effect/effect.h"
#include "azer/render/camera.h"
#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"

namespace azer {
AxesFrame::AxesFrame() {
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  EntityDataPtr data(new EntityData(effect_->vertex_desc(), 6));
  VertexPack vpack(data->vdata());
  vpack.first();

  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(1.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 1.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 1.0f, 1.0f), VertexPos(0, 0));

  entity_ = new Entity(data);
  entity_->AddSubset(Subset(0, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(2, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(4, 2, 0, 0, kLineList));
}

void AxesFrame::Update(const Camera* camera, const Vector3& position) {
  effect_->SetPV(camera->GetProjViewMatrix());
  Matrix4 mat = std::move(std::move(Translate(position)) * scale_);
  effect_->SetWorld(mat);
}

void AxesFrame::SetColor(int32 index, const Vector4& color) {
  DCHECK(index, static_cast<int32>(arraysize(colors_)));
  colors_[index] = color;
}

void AxesFrame::Render(Renderer* renderer) {
  for (uint32 i = 0; i < arraysize(colors_); ++i) {
    effect_->SetAmbient(colors_[i]);
    renderer->BindEffect(effect_);
    entity_->DrawSub(i, renderer);
  }
}
}  // namespace azer
