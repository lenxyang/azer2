#include "azer/util/interactive/spotlight_controller.h"

#include "azer/effect/color_effect.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/util/geometry/geometry.h"
#include "azer/util/interactive/interactive_context.h"

namespace azer {
namespace {
static const float kTopRadius = 0.5f;
}
SpotLightControllerObj::SpotLightControllerObj()
    : theta_(),
      phi_(),
      range_(1.0f) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  InitCircle();
  InitBarrel();
}

SpotLightControllerObj::~SpotLightControllerObj() {}

void SpotLightControllerObj::InitCircle() {
}

void SpotLightControllerObj::InitBarrel() {
  Matrix4 mat = std::move(RotateX(Degree(-90.0f)));
  float top_radius = kTopRadius;
  float inner_sine = std::sqrt(1 - theta_ * theta_);
  float inner_radius = range_ * inner_sine / theta_;
  float outer_sine = std::sqrt(1 - phi_ * phi_);
  float outer_radius = range_ * outer_sine / phi_;

  Matrix4 rot = std::move(RotateX(Degree(-90.0f)));
  GeoCylinderParam p;
  p.height = range_;
  p.top_radius = top_radius();
  p.bottom_radius = outer_radius;
  AppendGeoCylinderData(data, p, rot);
  p.bottom_radius = inner_radius;
  AppendGeoCylinderData(data, p, rot);
}

void SpotLightControllerObj::Update(const Camera* camera, const Vector3& position) {
  effect_->SetPV(camera->GetProjViewMatrix());
  effect_->SetWorld(Translate(position));
}

void SpotLightControllerObj::Render(Renderer* renderer) {
  renderer->BindEffect(effect_);
  entity_->Draw(renderer);
}

// class SpotLightController
SpotLightController::SpotLightController(InteractiveContext* ctx)
    : InteractiveController(ctx) {
}

SpotLightController::~SpotLightController() {
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
