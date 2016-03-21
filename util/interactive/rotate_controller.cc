#include "azer/util/interactive/rotate_controller.h"

#include "base/logging.h"
#include "azer/math/plane.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"
#include "azer/util/geometry/geometry.h"
#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/pick_util.h"

namespace azer {
// class RotateControlerObj
const float RotateControllerObj::kAxisLength = 0.6f;; 
RotateControllerObj::RotateControllerObj() {
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  InitEntity();
  ResetColor();
}

void RotateControllerObj::InitEntity() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  ambient_effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
  color_effect_ = (ColorEffect*)env->GetEffect("ColorEffect");

  VertexDataPtr vdata(new VertexData(ambient_effect_->vertex_desc(), 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata, idata));
  AppendAxisData(data);
  AppendRoundData(data);
  entity_ = new Entity(data);
}

void RotateControllerObj::AppendRoundData(EntityData* data) {
  GeoSphereParam param;
  param.slice = 64;
  param.stack = 32;
  Matrix4 xroundmat = RotateZ(Degree(90.0f));
  Matrix4 yroundmat = Matrix4::kIdentity;
  Matrix4 zroundmat = RotateX(Degree(-90.0f));
  AppendGeoSphereData(data, param, Matrix4::kIdentity);
  AppendGeoCircleData(data, param.radius + 0.01f, 1024, xroundmat);
  AppendGeoCircleData(data, param.radius + 0.01f, 1024, yroundmat);
  AppendGeoCircleData(data, param.radius + 0.01f, 1024, zroundmat);
}

void RotateControllerObj::AppendAxisData(EntityData* data) {
  const int32 kVertexCount = 6;
  data->vdata()->extend(kVertexCount);
  int32 base = data->vdata()->vertex_count() - kVertexCount;
  VertexPack vpack(data->vdata());
  vpack.move(base);
  VertexPos vpos(0, 0);
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kAxisLength, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kAxisLength, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kAxisLength, 1.0f), vpos);

  data->AddSubset(Subset(base, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 2, 2, 0, 0, kLineList));
  data->AddSubset(Subset(base + 4, 2, 0, 0, kLineList));
}

void RotateControllerObj::SetColor(int32 index, const Vector4& c) {
  DCHECK_LT(index, static_cast<int32>(arraysize(colors_)));
  colors_[index] = c;
}

void RotateControllerObj::ResetColor() {
  colors_[kAxisX]  = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
  colors_[kAxisY]  = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
  colors_[kAxisZ]  = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
  colors_[kRoundX] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  colors_[kRoundY] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  colors_[kRoundZ] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  colors_[kSphere] = Vector4(0.80f, 0.8f, 0.8f, 0.4f);
}

void RotateControllerObj::Update(const Camera* camera, const Vector3& position) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Matrix4 mat = std::move(Scale(scale_));
  mat = std::move(std::move(Translate(position)) * mat);
  ambient_effect_->SetPV(camera->GetProjViewMatrix());
  ambient_effect_->SetWorld(mat);
  color_effect_->SetLightData(&env->light()->data(), 1);
  color_effect_->SetPV(camera->GetProjViewMatrix());
  color_effect_->SetWorld(mat);
}

void RotateControllerObj::Render(Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  ScopedResetBlending scoped_blending(renderer);
  for (uint32 i = 0; i < arraysize(colors_); ++i) {
    if (i == kSphere) {
      renderer->SetBlending(env->blending(), 0, 0xffffffff);
      ColorMaterialData mtrl;
      mtrl.diffuse = colors_[i];
      mtrl.ambient = colors_[i] * 0.1f;
      mtrl.specular = colors_[i] * 0.1f;
      mtrl.emission = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
      mtrl.alpha = colors_[i].w;
      color_effect_->SetMaterial(mtrl);
      renderer->BindEffect(color_effect_);
      entity_->DrawSub(i, renderer);
    } else {
      ambient_effect_->SetAmbient(colors_[i]);
      renderer->BindEffect(ambient_effect_);
      entity_->DrawSub(i, renderer);
    }
  }
}

// class RotateController
RotateController::RotateController(InteractiveContext* ctx) 
    : InteractiveController(ctx) {
  object_.reset(new RotateControllerObj);
  scale_ = Vector3(1.0f, 1.0f, 1.0);
  position_ = Vector3(0.0f, 0.0f, 0.0f);
}

RotateController::~RotateController() {
}

int32 RotateController::GetPicking(const gfx::Point& pt) {
  return 0;
}

void RotateController::UpdateFrame(const FrameArgs& args) {
  object_->SetScale(scale_);
  object_->Update(context()->camera(), position_);
}

void RotateController::RenderFrame(Renderer* renderer) {
  object_->Render(renderer);
}
}  // namespace azer
