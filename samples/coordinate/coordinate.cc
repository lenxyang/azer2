#include "azer/samples/coordinate/coordinate.h"

#include "azer/math/math.h"
#include "azer/render/util.h"

namespace azer {
CoordinateAxis::CoordinateAxis(const DirLight* light)
    : light_(light) {
  color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  diffuse_effect_ = CreateColoredDiffuseEffect();

  cone_ = new ConeObject(diffuse_effect_->GetVertexDesc(), 16);
  cylinder_ = new CylinderObject(diffuse_effect_->GetVertexDesc(), 16, 16);

  // cone_world_ = Scale(0.18f, 0.12f, 0.18f);
  cone_world_ = Scale(0.075f, 0.2f, 0.075f);
  cone_world_ = std::move(Translate(0.0f, 0.80f, 0.0f) * cone_world_);

  // cylinder_world_ = Scale(0.12f, 0.88f, 0.12f);
  cylinder_world_ = Scale(.05f, 0.80f, 0.05f);
}

CoordinateAxis::~CoordinateAxis() {
}

void CoordinateAxis::Update(const Camera& camera) {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera.GetProjViewMatrix() * world_);
}

void CoordinateAxis::Render(Renderer* renderer) {
  diffuse_effect_->SetDirLight(*light_);
  diffuse_effect_->SetColor(Vector4(1.0, 0.0, 0.0, 1.0));
  
  diffuse_effect_->SetWorld(world_ * cone_world_);
  diffuse_effect_->SetPVW(pvw_ * cone_world_);
  diffuse_effect_->Use(renderer);
  cone_->Render(renderer);

  diffuse_effect_->SetWorld(world_ * cylinder_world_);
  diffuse_effect_->SetPVW(pvw_ * cylinder_world_);
  diffuse_effect_->Use(renderer);
  cylinder_->Render(renderer);
}

CoordinateObject::CoordinateObject() {
  light_.dir = azer::Vector4(-0.6f, -0.6f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  diffuse_effect_ = CreateColoredDiffuseEffect();
  sphere_ = new SphereObject(diffuse_effect_->GetVertexDesc(), 16, 16);
  cone_ = new ConeObject(diffuse_effect_->GetVertexDesc(), 16);
  cylinder_ = new CylinderObject(diffuse_effect_->GetVertexDesc(), 16, 16);
}

CoordinateObject::~CoordinateObject() {
}

void CoordinateObject::Update(const Camera& camera) {
}

void CoordinateObject::Render(Renderer* renderer) {
}
}  // namespace azer
