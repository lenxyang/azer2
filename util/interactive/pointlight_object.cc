#include "azer/util/interactive/pointlight_object.h"

#include "azer/effect/color_effect.h"

#include "azer/effect/material.h"
#include "azer/render/camera.h"
#include "azer/render/entity.h"
#include "azer/render/blending.h"
#include "azer/render/render_system.h"
#include "azer/util/interactive/env.h"

namespace azer {
PointLightObject::PointLightObject() 
    : scale_(1.0f, 1.0f, 1.0f),
      color_(1.0f, 1.0f, 1.0f, 1.0f),
      range_(5.0f), 
      alpha_(0.2f),
      show_range_(false) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  graypoint_.reset(new GrayPoint);
  effect_ = (ColorEffect*)env->GetEffect("ColorEffect");
  EntityDataPtr entitydata = CreateSphere(effect_->vertex_desc(), GeoSphereParam());
  entity_ = new Entity(entitydata.get());
}

PointLightObject::~PointLightObject() {
}

void PointLightObject::Render(const Camera& camera, Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  if (show_range_) {
    graypoint_->set_position(position_);
    graypoint_->set_scale(scale_);
    graypoint_->set_color(color_);
    graypoint_->Render(camera, renderer);
  }

  ColorMaterialData mtrl;
  mtrl.specular = mtrl.diffuse = color_;
  mtrl.alpha = alpha_;

  Matrix4 world = std::move(Translate(position_)) * 
      std::move(Scale(range_, range_, range_));
  ScopedResetBlending scoped_blending(renderer);
  ScopedRasterizerState scoped_raster(renderer);
  renderer->SetRasterizerState(env->noncull_rasterizer_state());
  renderer->SetBlending(env->blending(), 0, 0xffffffff);
  effect_->SetWorld(world);
  effect_->SetPV(camera.GetProjViewMatrix());
  effect_->SetMaterial(mtrl);
  effect_->SetLightData(&env->light()->data(), 1);
  renderer->BindEffect(effect_.get());
  entity_->Draw(renderer);
}
}  // namespace azer
