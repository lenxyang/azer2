#include "azer/util/interactive/translate_controller.h"

#include "base/logging.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"

namespace azer {
// class TranslateControlObj
TranslateControlObj::TranslateControlObj() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
  // texeffect_ = (TextBillboardEffect*)env->GetEffect("TextBillboardEffect");
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  scoped_refptr<EntityData> data(new EntityData(effect_->vertex_desc(), 32));
  VertexPack vpack(data->vdata());
  vpack.first();
  
  VertexPos vpos(0, 0);
  vpack.WriteVector3Or4(Vector4(0.3f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(1.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.3f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 1.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.3f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 1.0f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // XY & XZ
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // YZ & YZ
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // ZX && ZY
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // XY
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // YZ
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.5f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // ZX
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.5f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.5f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));

  entity_ = new Entity(data);
  entity_->AddSubset(Subset(0, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(2, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(4, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(6, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(8, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(10, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(12, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(14, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(16, 2, 0, 0, kLineList));

  entity_->AddSubset(Subset(20, 4, 0, 0, kTriangleStrip));
  entity_->AddSubset(Subset(24, 4, 0, 0, kTriangleStrip));
  entity_->AddSubset(Subset(28, 4, 0, 0, kTriangleStrip));
  ResetColor();
}

void TranslateControlObj::ResetColor() {
  colors_[kAxisX] = colors_[kLineXY] = colors_[kLineXZ] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  colors_[kAxisY] = colors_[kLineYX] = colors_[kLineYZ] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  colors_[kAxisZ] = colors_[kLineZX] = colors_[kLineZY] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  colors_[kPlaneXY] = colors_[kPlaneYZ] = colors_[kPlaneZX] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void TranslateControlObj::Update(const Camera* camera, const Vector3& position) {
  effect_->SetPV(camera->GetProjViewMatrix());
  Matrix4 mat = std::move(Scale(scale_));
  mat = std::move(std::move(Translate(position)) * mat);
  effect_->SetWorld(mat);
}

void TranslateControlObj::Render(Renderer* renderer) {
  for (uint32 i = 0; i < arraysize(colors_); ++i) {
    effect_->SetAmbient(colors_[i]);
    renderer->BindEffect(effect_);
    entity_->DrawSub(i, renderer);
  }
}


// class TranslateController
TranslateController::TranslateController(const Vector3& initpos) 
    : initpos_(initpos) {
}

TranslateController::~TranslateController() {
}

int32 TranslateController::GetPicking(const Ray& ray, Vector3* pos) {
  return 0;
}

void TranslateController::UpdateFrame(const FrameArgs& args) {
}

void TranslateController::RenderFrame(Renderer* renderer) {
}
}  // namespace azer
