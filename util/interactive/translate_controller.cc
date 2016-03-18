#include "azer/util/interactive/translate_controller.h"

#include "base/logging.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"
#include "azer/util/geometry/geometry.h"

namespace azer {
// class TranslateControlObj
TranslateControlObj::TranslateControlObj() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
  // texeffect_ = (TextBillboardEffect*)env->GetEffect("TextBillboardEffect");
  scale_ = Vector3(1.0f, 1.0f, 1.0f);

  VertexDataPtr vdata(new VertexData(effect_->vertex_desc(), 1));
  IndicesDataPtr idata(new IndicesData(1));
  scoped_refptr<EntityData> data(new EntityData(vdata, idata));
  InitAxisObjects(data.get());
  GeoConeParam p;
  p.radius = 0.08f;
  p.height = 0.3f;
  Matrix4 xmat = RotateZ(Degree(-90.0f)) * Translate(0.0f, 1.0f, 0.0f);
  Matrix4 ymat = Translate(0.0f, 1.0f, 0.0f);
  Matrix4 zmat = RotateZ(Degree(90.0f)) * Translate(0.0f, 1.0f, 0.0f);
  AppendGeoConeData(data.get(), p, xmat);
  AppendGeoConeData(data.get(), p, ymat);
  AppendGeoConeData(data.get(), p, zmat);

  entity_ = new Entity(data);
  ResetColor();
}

void TranslateControlObj::InitAxisObjects(EntityData* data) {
  VertexPack vpack(data->vdata());
  const int32 kVertexCount = 32;
  data->vdata()->extend(kVertexCount);
  int32 base = data->vdata()->vertex_count() - kVertexCount;
  vpack.move(base);
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

  
  data->AddSubset(Subset(base + 0, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 2, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 4, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 6, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 8, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 10, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 12, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 14, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 16, 2, 0, 0, kLineList));

  data->AddSubset(Subset(base + 20, 4, 0, 0, kTriangleStrip));
  data->AddSubset(Subset(base + 24, 4, 0, 0, kTriangleStrip));
  data->AddSubset(Subset(base + 28, 4, 0, 0, kTriangleStrip));
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
