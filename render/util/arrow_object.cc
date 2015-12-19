#include "azer/render/util/arrow_object.h"

#include "azer/render/geometry/cone_object.h"
#include "azer/render/geometry/cylinder_object.h"

namespace azer {
ArrowObject::ArrowObject() {
  effect_ = CreateColoredDiffuseEffect();
  Init(effect_);
}

ArrowObject::ArrowObject(ColoredDiffuseEffectPtr effect) 
    : effect_(effect) {
  Init(effect);
}

ArrowObject::~ArrowObject() {
}

void ArrowObject::Init(ColoredDiffuseEffectPtr effect) {
  light_.dir = azer::Vector4(-0.6f, -0.6f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  cone_ = new ConeObject(effect->vertex_desc(), 16);
  cylinder_ = new CylinderObject(effect->vertex_desc(), 16, 16);

  
  color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  cone_world_ = Scale(0.06f, 0.2f, 0.06f);
  cone_world_ = std::move(Translate(0.0f, 0.80f, 0.0f) * cone_world_);
  cylinder_world_ = Scale(.04f, 0.80f, 0.04f);
}

void ArrowObject::Render(const Matrix4& world,  const Matrix4& pvw, 
                         Renderer* renderer) {
  ColoredDiffuseEffect* effect = effect_.get();
  effect->SetDirLight(light_);
  effect->SetColor(color_);
  effect->SetWorld(world * cone_world_);
  effect->SetPVW(pvw * cone_world_);
  renderer->UseEffect(effect);
  cone_->Render(renderer);

  effect->SetWorld(world * cylinder_world_);
  effect->SetPVW(pvw * cylinder_world_);
  renderer->UseEffect(effect);
  cylinder_->Render(renderer);
}
}  // namespace azer
