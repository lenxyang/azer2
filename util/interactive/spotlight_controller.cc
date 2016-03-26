#include "azer/util/interactive/spotlight_controller.h"

#include "azer/effect/color_effect.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/geometry.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/interactive_context.h"

namespace azer {
namespace {
static const float kTopRadius = 0.5f;
}
SpotLightControllerObj::SpotLightControllerObj(Light* light) {
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

SpotLightControllerObj::~SpotLightControllerObj() {}

void SpotLightControllerObj::InitCircle() {
}

void SpotLightControllerObj::InitBarrel() {
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
  p.bottom_radius = outer_radius;
  {
    p.bottom_radius = inner_radius;
    VertexDataPtr vdata(new VertexData(color_effect_->vertex_desc(), 1));
    IndicesDataPtr idata(new IndicesData(1));
    EntityDataPtr data(new EntityData(vdata, idata));
    AppendGeoBarrelData(data, p, mat);
    inner_object_ = new Entity(data);
  }

  {
    VertexDataPtr vdata(new VertexData(color_effect_->vertex_desc(), 1));
    IndicesDataPtr idata(new IndicesData(1));
    EntityDataPtr data(new EntityData(vdata, idata));
    AppendGeoCylinderData(data, p, mat);
    outer_object_ = new Entity(data);
  }
}

void SpotLightControllerObj::Update(const Camera* camera) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Matrix4 mat = std::move(Translate(position_) * orientation_.ToMatrix());
  color_effect_->SetPV(camera->GetProjViewMatrix());
  color_effect_->SetWorld(mat);
  color_effect_->SetLightData(&env->light()->data(), 1);
}

void SpotLightControllerObj::Render(Renderer* renderer) {
  ScopedResetBlending scoped_blending(renderer);
  renderer->SetBlending(blending_, 0, 0xffffffff);
  ColorMaterialData mtrl;
  mtrl.diffuse = inner_color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.alpha = mtrl.diffuse.w;
  color_effect_->SetMaterial(mtrl);
  renderer->BindEffect(color_effect_);
  inner_object_->Draw(renderer);

  mtrl.diffuse = outer_color_;
  mtrl.ambient = mtrl.diffuse * 0.4f;
  mtrl.specular = mtrl.diffuse * 0.1f;
  mtrl.alpha = mtrl.diffuse.w;
  color_effect_->SetMaterial(mtrl);
  renderer->BindEffect(color_effect_);
  outer_object_->Draw(renderer);
}

// class SpotLightController
SpotLightController::SpotLightController(Light* light, InteractiveContext* ctx)
    : InteractiveController(ctx),
    light_(light) {
  object_.reset(new SpotLightControllerObj(light));
}

SpotLightController::~SpotLightController() {
}

void SpotLightController::SetLight(Light* ptr) {
  object_.reset(new SpotLightControllerObj(ptr));
  DCHECK_EQ(ptr->type(), kSpotLight);
  light_ = ptr;
}

int32 SpotLightController::GetPicking(const gfx::Point& pt) {
  return 0;
}

void SpotLightController::OnDragBegin(const gfx::Point& pt) {
}

void SpotLightController::OnDrag(const gfx::Point& pt) {
}

void SpotLightController::OnDragEnd(const gfx::Point& pt) {
}

void SpotLightController::UpdateFrame(const FrameArgs& args) {
  const UniverseLight& data = light_->data();
  Vector3 position(data.position[0], data.position[1], data.position[2]);
  object_->set_inner_color(Vector4(0.75f, 0.75f, 0.75f, 0.2f));
  object_->set_outer_color(Vector4(0.9f,   0.9f,  0.9f, 0.4f));
  object_->set_position(position);
  object_->Update(context()->camera());
}

void SpotLightController::RenderFrame(Renderer* renderer) {
  object_->Render(renderer);
}

void SpotLightController::AddSpotLightObserver(SpotLightControllerObserver* obs) {
  observer_list_.AddObserver(obs);
}

void SpotLightController::RemoteSpotLightObserver(SpotLightControllerObserver* obs) {
  observer_list_.RemoveObserver(obs);
}

bool SpotLightController::HasSpotLightObserver(SpotLightControllerObserver* obs) {
  return observer_list_.HasObserver(obs);
}
}  // namespace azer
