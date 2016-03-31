#include "azer/util/interactive/spotlight_controller.h"

#include "base/logging.h"
#include "azer/effect/color_effect.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/geometry.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/interactive_context.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {

SpotLightObject::SpotLightObject(Light* light)
    : light_(light) {
  DCHECK_EQ(light->type(), kSpotLight);
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  scale_ = Vector3(1.0f, 1.0f, 1.0f);

  const float kConeHeight = 0.5f;
  const float kCylinderHeight = 0.5f;
  VertexDataPtr vdata(new VertexData(effect_->vertex_desc(), 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata, idata));
  Matrix4 rot = std::move(RotateX(Degree(-90.0f)));
  {
    GeoCylinderParam param;
    param.top_radius = 0.25f;
    param.bottom_radius = param.top_radius * 0.5f;
    param.height = kConeHeight;
    float depth = -kConeHeight;
    Matrix4 mat = Translate(0.0f, 0.0f, depth) * rot;
    AppendGeoCylinderSubset(data, param, mat);
  }

  {
    GeoCylinderParam param;
    param.bottom_radius = param.top_radius = 0.1f;
    param.height = kCylinderHeight;
    float depth = 0.0f - kCylinderHeight - kConeHeight;
    Matrix4 mat = Translate(0.0f, 0.0f, depth) * rot;
    AppendGeoCylinderSubset(data, param, mat);    
  }

  entity_ = new Entity(data);
}

SpotLightObject::~SpotLightObject() {
}

void SpotLightObject::Render(const Camera& camera, Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();

  Quaternion quad;
  CalcSceneOrientForZDirection(light_->directional(), &quad);
  Matrix4 world_ = std::move(Translate(light_->position()))
      * std::move(quad.ToMatrix())
      * std::move(Scale(scale_));
  
  ColorMaterialData mtrl;
  mtrl.diffuse = light_->diffuse();
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.emission = mtrl.diffuse * 0.4f;
  mtrl.alpha = 1.0f;
  effect_->SetMaterial(mtrl);
  effect_->SetWorld(world_);
  effect_->SetPV(camera.GetProjViewMatrix());
  effect_->SetLightData(&env->light()->data(), 1);
  renderer->BindEffect(effect_);
  entity_->Draw(renderer);
}

namespace {
static const float kTopRadius = 0.5f;
}
SpotLightDirectionalObject::SpotLightDirectionalObject(Light* light) {
  DCHECK_EQ(light->type(), kSpotLight);
  const UniverseLight& data = light->data();
  theta_ = data.spotarg.theta;
  phi_ = data.spotarg.phi;
  range_ = data.spotarg.range;
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  InitCircle();
  InitBarrel();
  blending_ = env->blending();
}

SpotLightDirectionalObject::~SpotLightDirectionalObject() {}

void SpotLightDirectionalObject::InitCircle() {
}

void SpotLightDirectionalObject::InitBarrel() {
  float top_radius = kTopRadius;
  float inner_sine = std::sqrt(1 - theta_ * theta_);
  float inner_radius = range_ * inner_sine / theta_;
  float outer_sine = std::sqrt(1 - phi_ * phi_);
  float outer_radius = range_ * outer_sine / phi_;
  
  Matrix4 mat = std::move(RotateX(Degree(-90.0f)))
      * std::move(Translate(0.0f, -range_, 0.0f));
  GeoCylinderParam p;
  p.height = range_;
  p.stack = 5;
  p.top_radius = top_radius;
  {
    p.bottom_radius = inner_radius;
    VertexDataPtr vdata(new VertexData(color_effect_->vertex_desc(), 1));
    IndicesDataPtr idata(new IndicesData(1));
    EntityDataPtr data(new EntityData(vdata, idata));
    AppendGeoBarrelSubset(data, p, mat);
    inner_object_ = new Entity(data);
  }

  {
    p.bottom_radius = outer_radius;
    VertexDataPtr vdata(new VertexData(color_effect_->vertex_desc(), 1));
    IndicesDataPtr idata(new IndicesData(1));
    EntityDataPtr data(new EntityData(vdata, idata));
    AppendGeoCylinderSubset(data, p, mat);
    outer_object_ = new Entity(data);
  }
}

void SpotLightDirectionalObject::Update(const Camera& camera) {
  pv_ = camera.GetProjViewMatrix();
  world_ = std::move(Translate(position_) * orientation_.ToMatrix());
}

void SpotLightDirectionalObject::Render(Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_effect_->SetPV(pv_);
  color_effect_->SetWorld(world_);
  color_effect_->SetLightData(&env->light()->data(), 1);

  ScopedResetBlending scoped_blending(renderer);
  ScopedRasterizerState scoped_state(renderer);
  renderer->SetBlending(env->blending(), 0, 0xffffffff);
  renderer->SetRasterizerState(env->noncull_rasterizer_state());
  ColorMaterialData inner_mtrl;
  inner_mtrl.diffuse = inner_color_;
  inner_mtrl.ambient = inner_mtrl.diffuse * 0.4f;
  inner_mtrl.specular = inner_mtrl.diffuse * 0.1f;
  inner_mtrl.alpha = inner_mtrl.diffuse.w;
  ColorMaterialData outer_mtrl;
  outer_mtrl.diffuse = outer_color_;
  outer_mtrl.ambient = outer_mtrl.diffuse * 0.4f;
  outer_mtrl.specular = outer_mtrl.diffuse * 0.1f;
  outer_mtrl.alpha = outer_mtrl.diffuse.w;

  color_effect_->SetMaterial(inner_mtrl);
  renderer->BindEffect(color_effect_);
  inner_object_->Draw(renderer);
  color_effect_->SetMaterial(outer_mtrl);
  renderer->BindEffect(color_effect_);
  outer_object_->Draw(renderer);
}

// class SpotLightController
SpotLightController::SpotLightController(InteractiveContext* ctx)
    : InteractiveController(ctx),
      mode_(kNone) ,
      new_mode_(kNone) {
  translate_controller_.reset(new TranslateController(ctx));
  rotate_controller_.reset(new RotateController(ctx));
}

SpotLightController::~SpotLightController() {}

void SpotLightController::SetLight(Light* ptr) {
  object_.reset(new SpotLightDirectionalObject(ptr));
  DCHECK_EQ(ptr->type(), kSpotLight);
  light_ = ptr;
}

void SpotLightController::set_mode(int32 mode) {
  new_mode_ = mode;
}

int32 SpotLightController::GetPicking(const gfx::Point& pt) {
  if (mode_ == kRotate) {
    return rotate_controller_->GetPicking(pt);
  } else if (mode_ == kTranslate) {
    return translate_controller_->GetPicking(pt);
  } else {
    return 0;
  }
}

void SpotLightController::OnDragBegin(const ui::MouseEvent& e) {
  if (mode_ == kRotate) {
    rotate_controller_->OnDragBegin(e);
  } else if (mode_ == kTranslate) {
    translate_controller_->OnDragBegin(e);
  } else {
  }
}

void SpotLightController::OnDragging(const ui::MouseEvent& e) {
  if (mode_ == kRotate) {
    rotate_controller_->OnDragging(e);
  } else if (mode_ == kTranslate) {
    translate_controller_->OnDragging(e);
  } else {
  }
}

void SpotLightController::OnDragEnd(const ui::MouseEvent& e) {
  if (mode_ == kRotate) {
    rotate_controller_->OnDragEnd(e);
  } else if (mode_ == kTranslate) {
    translate_controller_->OnDragEnd(e);
  } else {
  }
}

void SpotLightController::UpdateFrame(const FrameArgs& args) {
  Quaternion quad;
  CalcSceneOrientForZDirection(light_->directional(), &quad);
  object_->set_position(light_->position());
  object_->set_orientation(quad);
  object_->Update(*context()->camera());
  translate_controller_->set_scale(Vector3(0.5f, 0.5f, 0.5f));
  translate_controller_->set_position(light_->position());
  rotate_controller_->set_scale(Vector3(0.5f, 0.5f, 0.5f));
  rotate_controller_->set_position(light_->position());

  rotate_controller_->UpdateFrame(args);
  translate_controller_->UpdateFrame(args);
}

void SpotLightController::RenderFrame(Renderer* renderer) {
  if (mode_ == kRotate) {
    rotate_controller_->RenderFrame(renderer);

    object_->set_inner_color(Vector4(0.75f, 0.75f, 0.75f, 0.2f));
    object_->set_outer_color(Vector4(0.9f,   0.9f,  0.9f, 0.4f));
    object_->set_position(light_->position());
    object_->Update(*context()->camera());
    object_->Render(renderer);
  } else if (mode_ == kTranslate) {
    translate_controller_->RenderFrame(renderer);
  } else {
  }
  
  mode_ = new_mode_;
}

void SpotLightController::AddSpotLightObserver(SpotLightControllerObserver* obs) {
  observer_list_.AddObserver(obs);
}

void SpotLightController::RemoveSpotLightObserver(SpotLightControllerObserver* obs) {
  observer_list_.RemoveObserver(obs);
}

bool SpotLightController::HasSpotLightObserver(SpotLightControllerObserver* obs) {
  return observer_list_.HasObserver(obs);
}

void SpotLightController::OnRotateBegin(RotateController* controller) {
}

void SpotLightController::OnRotating(RotateController* controller) {
}

void SpotLightController::OnRotateEnd(RotateController* controller) {
}

void SpotLightController::OnTranslateBegin(TranslateController* controller) {
}

void SpotLightController::OnTranslating(TranslateController* controller) {
}

void SpotLightController::OnTranslateEnd(TranslateController* controller) {
}
}  // namespace azer
