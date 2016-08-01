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
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  EntityDataPtr axes(InitAxesObjects(ambient_effect_->vertex_desc()));
  EntityDataPtr cone(InitAxesConeData(ambient_effect_->vertex_desc()));
  entity_ = new Entity(axes.get());
  cones_ = new Entity(cone.get());
  ResetColor();
}

EntityDataPtr TranslateControlObj::InitAxesConeData(VertexDesc* desc) {
  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));

  GeoConeParam p;
  p.radius = 0.03f;
  p.height = 0.25f;
  Matrix4 xmat = Transform::RotateZ(Degree(90.0f))
      * Transform::Translate(0.0f, 1.0f, 0.0f);
  Matrix4 ymat = Transform::Translate(0.0f, 1.0f, 0.0f);
  Matrix4 zmat = Transform::RotateX(Degree(-90.0f))
      * Transform::Translate(0.0f, 1.0f, 0.0f);
  AppendGeoConeData(data.get(), p, xmat);
  AppendGeoConeData(data.get(), p, ymat);
  AppendGeoConeData(data.get(), p, zmat);
  return data;
}

EntityDataPtr TranslateControlObj::InitAxesObjects(VertexDesc* desc) {
  scoped_refptr<EntityData> data(new EntityData(desc, 1));

  VertexPack vpack(data->vdata());
  const int32_t kVertexCount = 32;
  data->vdata()->extend(kVertexCount);
  int32_t base = data->vdata()->vertex_count() - kVertexCount;
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

  // planes
  // XY
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));

  // YZ
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, kPlaneWidth, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));

  // ZX
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(kPlaneWidth, 0.0f, kPlaneWidth, 1.0f), vpos);
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), vpos);
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

  data->AddSubset(Subset(base + 18, 4, 0, 0, kTriangleStrip));
  data->AddSubset(Subset(base + 22, 4, 0, 0, kTriangleStrip));
  data->AddSubset(Subset(base + 26, 4, 0, 0, kTriangleStrip));
  return data;
}

float TranslateControlObj::plane_width() const {
  return kPlaneWidth;
}

float TranslateControlObj::axis_begin() const {
  return kAxisBegin;
}

void TranslateControlObj::ResetColor() {
  colors_[kAxisX] = colors_[kLineXY] = colors_[kLineXZ] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  colors_[kAxisY] = colors_[kLineYX] = colors_[kLineYZ] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  colors_[kAxisZ] = colors_[kLineZX] = colors_[kLineZY] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  colors_[kPlaneXY] = Vector4(1.0f, 0.0f, 0.0f, 0.2f);
  colors_[kPlaneYZ] = Vector4(0.0f, 1.0f, 0.0f, 0.2f);
  colors_[kPlaneZX] = Vector4(0.0f, 0.0f, 1.0f, 0.2f);
}

void TranslateControlObj::Update(const Camera& camera, const Vector3& position) {
  Matrix4 mat = std::move(Transform::Scale(scale_));
  world_ = std::move(std::move(Transform::Translate(position)) * mat);
  pv_ = camera.GetProjViewMatrix();
}

void TranslateControlObj::Render(Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();

  ambient_effect_->SetPV(pv_);
  ambient_effect_->SetWorld(world_);
  color_effect_->SetLightData(&env->light()->data(), 1);
  color_effect_->SetPV(pv_);
  color_effect_->SetWorld(world_);

  ScopedRasterizerState scoped_noncull(renderer);
  renderer->SetRasterizerState(env->noncull_rasterizer_state());
  {
    ScopedResetBlending scoped_blending(renderer);
    ScopedDepthStencilState scoped_depth(renderer);
    renderer->SetDepthStencilState(env->depth_disabled_state(), 0x0);
    for (uint32_t i = 0; i < arraysize(colors_); ++i) {
      if (i == kPlaneXY) {
        renderer->SetBlending(env->a2c_blending(), 0, 0xffffffff);
      }
      ambient_effect_->SetAmbient(colors_[i]);
      renderer->BindEffect(ambient_effect_.get());
      entity_->DrawSub(i, renderer);
    }
  }

  ColorMaterialData mtrl;
  for (int32_t i = 0; i < cones_->subset_count() /2; ++i) {
    mtrl.diffuse = colors_[i];
    mtrl.ambient = colors_[i] * 0.1f;
    mtrl.specular = colors_[i] * 0.1f;
    mtrl.emission = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    mtrl.ambient.w = mtrl.specular.w = 1.0f;
    color_effect_->SetMaterial(mtrl);
    renderer->BindEffect(color_effect_.get());
    cones_->DrawSub(i * 2, renderer);
    cones_->DrawSub(i * 2 + 1, renderer);
  }
}


// class TranslateController
const Vector4 TranslateController::kSelectedColor = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
const Vector4 TranslateController::kSelectedPlaneColor = 
    Vector4(1.0f, 1.0f, 0.0f, 0.4f);
TranslateController::TranslateController(InteractiveContext* ctx)
    : InteractiveController(ctx) {
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  object_.reset(new TranslateControlObj);
}

TranslateController::~TranslateController() {
}

int32_t TranslateController::GetPicking(const gfx::Point& screenpt) {
  Ray ray = std::move(context()->GetPickingRay(screenpt));
  const float kMargin = 0.02 * scale_.x;
  const float kPlaneWidth = object_->plane_width() * scale_.x;
  Plane planexy(position_, position_ + Vector3(1.0f, 0.0f, 0.0f),
                position_ + Vector3(0.0f, 1.0f, 0.0f));
  Plane planeyz(position_, position_ + Vector3(0.0f, 1.0f, 0.0f),
                position_ + Vector3(0.0f, 0.0f, 1.0f));
  Plane planezx(position_, position_ + Vector3(0.0f, 0.0f, 1.0f),
                position_ + Vector3(1.0f, 0.0f, 0.0f));
  Vector3 xy, yz, zx;
  bool parall_xy = !PickingPlane(ray, planexy, &xy);
  bool parall_yz = !PickingPlane(ray, planeyz, &yz);
  bool parall_zx = !PickingPlane(ray, planezx, &zx);
  bool hit_axisx = (!parall_xy && !parall_zx)
      && (std::abs(xy.y - position_.y) < 0.02f)
      && (std::abs(xy.z - position_.z) < 0.02f);
  bool hit_axisy = (!parall_yz && !parall_xy)
      && (std::abs(xy.x - position_.x) < 0.02f)
      && (std::abs(xy.z - position_.z) < 0.02f);
  bool hit_axisz = (!parall_zx && !parall_yz)
      && (std::abs(xy.x - position_.x) < 0.02f)
      && (std::abs(xy.y - position_.y) < 0.02f);
  bool hit_planxy = (!parall_xy)
      && ((xy.x - position_.x) < kPlaneWidth)
      && ((xy.y - position_.y) < kPlaneWidth)
      && (xy.x >= position_.x) && (xy.y >= position_.y)
      && (std::abs(xy.x - position_.x) > 0.02f)
      && (std::abs(xy.y - position_.y) > 0.02f)
      && (std::abs(xy.z - position_.z) < 0.02f)
      && (xy.y - position_.y) >= (xy.x - position_.x);
  bool hit_planyz = (!parall_yz)
      && ((yz.y - position_.y) < kPlaneWidth)
      && ((yz.z - position_.z) < kPlaneWidth)
      && (yz.y >= position_.y) && (yz.z >= position_.z)
      && (std::abs(yz.x - position_.x) < 0.02f)
      && (std::abs(yz.y - position_.y) > 0.02f)
      && (std::abs(yz.z - position_.z) > 0.02f)
      && (yz.y - position_.y) >= (yz.x - position_.x);
  bool hit_planzx = (!parall_zx)
      && ((zx.z - position_.z) < kPlaneWidth)
      && ((zx.x - position_.x) < kPlaneWidth)
      && (zx.z >= position_.z) && (zx.x >= position_.x)
      && (std::abs(zx.x - position_.x) > 0.02f)
      && (std::abs(zx.y - position_.y) < 0.02f)
      && (std::abs(zx.z - position_.z) > 0.02f)
      && (zx.z - position_.z) >= (zx.x - position_.x);
  // float depth_xy = detail::CalcDepthValue(xy, *camera);
  // float depth_yz = detail::CalcDepthValue(yz, *camera);
  // float depth_zx = detail::CalcDepthValue(zx, *camera);

  if (hit_axisx) {
    return kHitAxisX;
  } else if (hit_axisy) {
    return kHitAxisY;
  } else if (hit_axisz) {
    return kHitAxisZ;
  } else if (hit_planxy) {
    return kHitPlaneXY;
  } else if (hit_planyz) {
    return kHitPlaneYZ;
  } else if (hit_planzx) {
    return kHitPlaneZX;
  } else {
    return kHitNone;
  }
}

void TranslateController::OnDragBegin(const ui::MouseEvent& e) {
  Vector3 offset;
  origin_position_ = position_;
  int state = GetPicking(e.location());
  set_state(state);
  Ray ray = std::move(context()->GetPickingRay(e.location()));
  GetDragInitPos(ray, &draginit_pos_);
  CalcDragOffset(ray, &offset);
  position_ = origin_position_ + offset;
  FOR_EACH_OBSERVER(TranslateControllerObserver, observer_list_, 
                    OnTranslateBegin(this));
}

void TranslateController::OnDragging(const ui::MouseEvent& e) {
  Ray ray = std::move(context()->GetPickingRay(e.location()));
  Vector3 offset;
  CalcDragOffset(ray, &offset);
  position_ = origin_position_ + offset;
  FOR_EACH_OBSERVER(TranslateControllerObserver, observer_list_, 
                    OnTranslating(this));
}

void TranslateController::OnDragEnd(const ui::MouseEvent& e) {
  Ray ray = std::move(context()->GetPickingRay(e.location()));
  Vector3 offset;
  CalcDragOffset(ray, &offset);
  position_ = origin_position_ + offset;

  FOR_EACH_OBSERVER(TranslateControllerObserver, observer_list_, 
                    OnTranslateEnd(this));
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
      object_->SetColor(TranslateControlObj::kPlaneXY, kSelectedPlaneColor);
      break;
    case kHitPlaneYZ:
      object_->SetColor(TranslateControlObj::kAxisY, kSelectedColor);
      object_->SetColor(TranslateControlObj::kAxisZ, kSelectedColor);
      object_->SetColor(TranslateControlObj::kPlaneYZ, kSelectedPlaneColor);
      break;
    case kHitPlaneZX:
      object_->SetColor(TranslateControlObj::kAxisZ, kSelectedColor);
      object_->SetColor(TranslateControlObj::kAxisX, kSelectedColor);
      object_->SetColor(TranslateControlObj::kPlaneZX, kSelectedPlaneColor);
      break;
    default:
    break;
  }
  object_->Update(*context()->camera(), position_);
}

void TranslateController::CalcDragOffset(const Ray& ray, Vector3* offset) {
  *offset = Vector3(0.0f, 0.0f, 0.0f);
  Vector3 pt;
  switch (state()) {
    case kHitAxisX: {
      Plane pxy(Vector3(0.0f, 0.0f, 1.0f), -origin_position_.z);
      PickingPlane(ray, pxy, &pt);
      offset->x = pt.x - draginit_pos_.x;
      offset->y = 0.0f;
      offset->z = 0.0f;
      break;
    }
    case kHitAxisY: {
      Plane pyz(Vector3(1.0f, 0.0f, 0.0f), -origin_position_.x);
      PickingPlane(ray, pyz, &pt);
      offset->x = 0.0f;
      offset->y = pt.y - draginit_pos_.y;
      offset->z = 0.0f;
      break;
    }
    case kHitAxisZ: {
      Plane pzx(Vector3(0.0f, 1.0f, 0.0f), -origin_position_.y);
      PickingPlane(ray, pzx, &pt);
      offset->x = 0.0f;
      offset->y = 0.0f;
      offset->z = pt.z - draginit_pos_.z;
      break;
    }
    case kHitPlaneXY: {
      Plane pxy(Vector3(0.0f, 0.0f, 1.0f), -origin_position_.z);
      PickingPlane(ray, pxy, &pt);
      offset->x = pt.x - draginit_pos_.x;
      offset->y = pt.y - draginit_pos_.y;
      offset->z = 0.0f;
      break;
    }
    case kHitPlaneYZ: {
      Plane pyz(Vector3(1.0f, 0.0f, 0.0f), -origin_position_.x);
      PickingPlane(ray, pyz, &pt);
      offset->x = 0.0f;
      offset->y = pt.y - draginit_pos_.y;
      offset->z = pt.z - draginit_pos_.z;
      break;
    }
    case kHitPlaneZX: {
      Plane pzx(Vector3(0.0f, 1.0f, 0.0f), -origin_position_.y);
      PickingPlane(ray, pzx, &pt);
      offset->x = pt.x - draginit_pos_.x;
      offset->y = 0.0f;
      offset->z = pt.z - draginit_pos_.z;
      break;
    }
    default: {
      CHECK(false);
      break;
    }
  }
}

void TranslateController::GetDragInitPos(const Ray& ray, Vector3* pos) {
  Vector3 pt;
  switch (state()) {
    case kHitAxisX: {
      Plane pxy(Vector3(0.0f, 0.0f, 1.0f), -position_.z);
      PickingPlane(ray, pxy, &pt);
      break;
    }
    case kHitAxisY: {
      Plane pyz(Vector3(1.0f, 0.0f, 0.0f), -position_.x);
      PickingPlane(ray, pyz, &pt);
      break;
    }
    case kHitAxisZ: {
      Plane pzx(Vector3(0.0f, 1.0f, 0.0f), -position_.y);
      PickingPlane(ray, pzx, &pt);
      break;
    }
    case kHitPlaneXY: {
      Plane pxy(Vector3(0.0f, 0.0f, 1.0f), -position_.z);
      PickingPlane(ray, pxy, &pt);
      break;
    }
    case kHitPlaneYZ: {
      Plane pyz(Vector3(1.0f, 0.0f, 0.0f), -position_.x);
      PickingPlane(ray, pyz, &pt);
      break;
    }
    case kHitPlaneZX: {
      Plane pzx(Vector3(0.0f, 1.0f, 0.0f), -position_.y);
      PickingPlane(ray, pzx, &pt);
      break;
    }
    default: {
      CHECK(false);
      break;
    }
  }
  *pos = pt;
}

void TranslateController::RenderFrame(Renderer* renderer) {
  object_->SetScale(scale_);
  object_->Render(renderer);
}

void TranslateController::AddTranslateObserver(TranslateControllerObserver* obs) {
  observer_list_.AddObserver(obs);
}

void TranslateController::RemoveTranslateObserver(TranslateControllerObserver* obs) {
  observer_list_.RemoveObserver(obs);
}

bool TranslateController::HasTranslateObserver(TranslateControllerObserver* obs) {
  return observer_list_.HasObserver(obs);
}
}  // namespace azer
