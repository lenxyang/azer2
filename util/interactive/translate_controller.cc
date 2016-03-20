#include "azer/util/interactive/translate_controller.h"

#include "base/logging.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/math/plane.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"
#include "azer/util/geometry/geometry.h"
#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/pick_util.h"


namespace azer {
// class TranslateControlObj
const float TranslateControlObj::kPlaneWidth = 0.35f;
const float TranslateControlObj::kAxisBegin = 0.27f;
TranslateControlObj::TranslateControlObj() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  ambient_effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
  color_effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  // color_effect__ = (TextBillboardEffect*)env->GetEffect("TextBillboardEffect");
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  entity_ = new Entity(InitAxesObjects(ambient_effect_->vertex_desc()));
  cones_ = new Entity(InitAxesConeData(ambient_effect_->vertex_desc()));
  ResetColor();
}

EntityDataPtr TranslateControlObj::InitAxesConeData(VertexDesc* desc) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata, idata));

  GeoConeParam p;
  p.radius = 0.03f;
  p.height = 0.25f;
  Matrix4 xmat = RotateZ(Degree(90.0f)) * Translate(0.0f, 1.0f, 0.0f);
  Matrix4 ymat = Translate(0.0f, 1.0f, 0.0f);
  Matrix4 zmat = RotateX(Degree(-90.0f)) * Translate(0.0f, 1.0f, 0.0f);
  AppendGeoConeData(data.get(), p, xmat);
  AppendGeoConeData(data.get(), p, ymat);
  AppendGeoConeData(data.get(), p, zmat);
  return data;
}

EntityDataPtr TranslateControlObj::InitAxesObjects(VertexDesc* desc) {
  scoped_refptr<EntityData> data(new EntityData(desc, 1));

  VertexPack vpack(data->vdata());
  const int32 kVertexCount = 32;
  data->vdata()->extend(kVertexCount);
  int32 base = data->vdata()->vertex_count() - kVertexCount;
  vpack.move(base);
  VertexPos vpos(0, 0);
  vpack.WriteVector3Or4(Vector4(kAxisBegin, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(1.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kAxisBegin, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 1.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kAxisBegin, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 1.0f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // XY & XZ
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));

  // YZ & YZ
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));

  // ZX && ZY
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));

  // XY
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // YZ
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));

  // ZX
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, 0.0f, 1.0f), vpos);
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
  return data;
}

float TranslateControlObj::plane_width() const {
  return kPlaneWidth * scale_.x;
}

float TranslateControlObj::axis_begin() const {
  return kAxisBegin * scale_.x;
}

void TranslateControlObj::ResetColor() {
  colors_[kAxisX] = colors_[kLineXY] = colors_[kLineXZ] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  colors_[kAxisY] = colors_[kLineYX] = colors_[kLineYZ] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  colors_[kAxisZ] = colors_[kLineZX] = colors_[kLineZY] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  colors_[kPlaneXY] = colors_[kPlaneYZ] = colors_[kPlaneZX] = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void TranslateControlObj::Update(const Camera* camera, const Vector3& position) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Matrix4 mat = std::move(Scale(scale_));
  mat = std::move(std::move(Translate(position)) * mat);
  ambient_effect_->SetPV(camera->GetProjViewMatrix());
  ambient_effect_->SetWorld(mat);
  color_effect_->SetLightData(&env->light()->data(), 1);
  color_effect_->SetPV(camera->GetProjViewMatrix());
  color_effect_->SetWorld(mat);
}

void TranslateControlObj::Render(Renderer* renderer) {
  for (uint32 i = 0; i < arraysize(colors_); ++i) {
    ambient_effect_->SetAmbient(colors_[i]);
    renderer->BindEffect(ambient_effect_);
    entity_->DrawSub(i, renderer);
  }

  ColorMaterialData mtrl;
  for (uint32 i = 0; i < cones_->subset_count() /2; ++i) {
    mtrl.diffuse = colors_[i];
    mtrl.ambient = colors_[i] * 0.1f;
    mtrl.specular = colors_[i] * 0.1f;
    mtrl.emission = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    mtrl.ambient.w = mtrl.specular.w = 1.0f;
    color_effect_->SetMaterial(mtrl);
    renderer->BindEffect(color_effect_);
    cones_->DrawSub(i * 2, renderer);
    cones_->DrawSub(i * 2 + 1, renderer);
  }
}


// class TranslateController
const Vector4 TranslateController::kSelectedColor = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
TranslateController::TranslateController(InteractiveContext* ctx)
    : InteractiveController(ctx) {
  object_.reset(new TranslateControlObj);
}

TranslateController::~TranslateController() {
}

int32 TranslateController::GetPicking(const gfx::Point& screenpt) {
  Ray ray = std::move(context()->GetPickingRay(screenpt));
  const Camera* camera = context()->camera(); 
  Plane planexy(position_, position_ + Vector3(1.0f, 0.0f, 0.0f),
                position_ + Vector3(0.0f, 1.0f, 0.0f));
  Plane planeyz(position_, position_ + Vector3(1.0f, 1.0f, 0.0f),
                position_ + Vector3(0.0f, 0.0f, 1.0f));
  Plane planezx(position_, position_ + Vector3(0.0f, 0.0f, 1.0f),
                position_ + Vector3(1.0f, 0.0f, 0.0f));
  Vector3 xy, yz, zx;
  bool parall_xy = PickingPlane(ray, planexy, &xy);
  bool parall_yz = PickingPlane(ray, planeyz, &yz);
  bool parall_zx = PickingPlane(ray, planezx, &zx);
  bool hit_axisx = (!parall_xy && !parall_zx)
      && (std::abs(xy.y - position_.y) < 0.0001)
      && (std::abs(xy.z - position_.z) < 0.0001);
  bool hit_axisy = (!parall_yz && !parall_xy)
      && (std::abs(xy.x - position_.x) < 0.0001)
      && (std::abs(xy.z - position_.z) < 0.0001);
  bool hit_axisz = (!parall_zx && !parall_yz)
      && (std::abs(xy.x - position_.x) < 0.0001)
      && (std::abs(xy.y - position_.y) < 0.0001);
  bool hit_planxy = (!parall_xy)
      && (std::abs(xy.x - position_.x) < object_->plane_width())
      && (std::abs(xy.y - position_.y) < object_->plane_width())
      && (std::abs(xy.x - position_.x) > 0.0f)
      && (std::abs(xy.y - position_.y) > 0.0f)
      && (std::abs(xy.z - position_.z) < 0.00001)
      && (xy.y - position_.y) >= (xy.x - position_.x);
  float depth_xy = detail::CalcDepthValue(xy, *camera);
  float depth_yz = detail::CalcDepthValue(yz, *camera);
  float depth_zx = detail::CalcDepthValue(zx, *camera);
  if (depth_xy <= depth_yz && depth_xy <= depth_zx) {
    if (hit_planxy) {
      return kHitPlaneXY;
    }
    if (hit_axisx) {
      return kHitAxisX;
    }
  }
  return kHitNone;
}

void TranslateController::UpdateFrame(const FrameArgs& args) {
  object_->ResetColor();
  switch (state()) {
    case kHitAxisX:
      object_->SetColor(TranslateControlObj::kAxisX, kSelectedColor);
      break;
    case kHitAxisY:
      object_->SetColor(TranslateControlObj::kAxisY, kSelectedColor);
      break;
    case kHitAxisZ:
      object_->SetColor(TranslateControlObj::kAxisZ, kSelectedColor);
      break;
    case kHitPlaneXY:
      object_->SetColor(TranslateControlObj::kAxisX, kSelectedColor);
      object_->SetColor(TranslateControlObj::kAxisY, kSelectedColor);
      object_->SetColor(TranslateControlObj::kPlaneXY, kSelectedColor);
      break;
    case kHitPlaneYZ:
    case kHitPlaneZX:
    default:
    break;
  }
  object_->Update(context()->camera(), position_);
}

void TranslateController::RenderFrame(Renderer* renderer) {
  object_->Render(renderer);
}
}  // namespace azer
