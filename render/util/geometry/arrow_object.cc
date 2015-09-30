#include "azer/render/util/geometry/arrow_object.h"

#include "azer/render/util/geometry/cone_object.h"
#include "azer/render/util/geometry/cylinder_object.h"

namespace azer {
ArrowObject::ArrowObject() {
  light_.dir = azer::Vector4(-0.6f, -0.6f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  diffuse_effect_ = CreateColoredDiffuseEffect();
  cone_ = new ConeObject(diffuse_effect_->GetVertexDesc(), 16);
  cylinder_ = new CylinderObject(diffuse_effect_->GetVertexDesc(), 16, 16);

  
  color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  cone_world_ = Scale(0.06f, 0.2f, 0.06f);
  cone_world_ = std::move(Translate(0.0f, 0.80f, 0.0f) * cone_world_);
  cylinder_world_ = Scale(.04f, 0.80f, 0.04f);
}

ArrowObject::~ArrowObject() {
}

void ArrowObject::Update(const Camera& camera) {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera.GetProjViewMatrix() * world_);
}

void ArrowObject::Render(Renderer* renderer) {
  ColoredDiffuseEffect* effect = diffuse_effect_.get();
  effect->SetDirLight(light_);
  effect->SetColor(color_);
  effect->SetWorld(world_ * cone_world_);
  effect->SetPVW(pvw_ * cone_world_);
  effect->Use(renderer);
  cone_->Render(renderer);

  effect->SetWorld(world_ * cylinder_world_);
  effect->SetPVW(pvw_ * cylinder_world_);
  effect->Use(renderer);
  cylinder_->Render(renderer);
}
}  // namespace azer
