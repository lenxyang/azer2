#include "azer/render/util/geometry/coordinate_object.h"

#include "azer/math/math.h"
#include "azer/render/util.h"

namespace azer {
CoordinateAxis::CoordinateAxis(const Matrix4& orientation, CoordinateObject* object)
    : orientation_(orientation),
      object_(object) {
  color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

  cone_world_ = Scale(0.06f, 0.2f, 0.06f);
  cone_world_ = std::move(Translate(0.0f, 0.80f, 0.0f) * cone_world_);
  cone_world_ = std::move(orientation * cone_world_);

  cylinder_world_ = Scale(.04f, 0.80f, 0.04f);
  cylinder_world_ = std::move(orientation * cylinder_world_);
}

CoordinateAxis::~CoordinateAxis() {
}

void CoordinateAxis::Render(const Matrix4& world,  const Matrix4& pvw,
                            Renderer* renderer) {
  GeometryObjectPtr cone = object_->cone_;
  GeometryObjectPtr cylinder = object_->cylinder_;
  ColoredDiffuseEffectPtr effect = object_->diffuse_effect_;
  effect->SetDirLight(object_->light_);
  effect->SetColor(color_);
  effect->SetWorld(world * cone_world_);
  effect->SetPVW(pvw * cone_world_);
  effect->Use(renderer);
  cone->Render(renderer);

  effect->SetWorld(world * cylinder_world_);
  effect->SetPVW(pvw * cylinder_world_);
  effect->Use(renderer);
  cylinder->Render(renderer);
}

CoordinateObject::CoordinateObject() {
  light_.dir = azer::Vector4(-0.6f, -0.6f, -0.2f, 0.0f);
  light_.diffuse = azer::Vector4(0.8f, 0.8f, 1.8f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  diffuse_effect_ = CreateColoredDiffuseEffect();
  sphere_ = new SphereObject(diffuse_effect_->GetVertexDesc(), 16, 16);
  cone_ = new ConeObject(diffuse_effect_->GetVertexDesc(), 16);
  cylinder_ = new CylinderObject(diffuse_effect_->GetVertexDesc(), 16, 16);

  sphere_ = new SphereObject(diffuse_effect_->GetVertexDesc(), 24, 24);
  Matrix4 xaxis_orientation = RotateZ(Degree(90.0f));
  Matrix4 zaxis_orientation = RotateX(Degree(-90.0f));
  
  xaxis_.reset(new CoordinateAxis(xaxis_orientation, this));
  xaxis_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  yaxis_.reset(new CoordinateAxis(Matrix4::kIdentity, this));
  yaxis_->SetColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
  zaxis_.reset(new CoordinateAxis(zaxis_orientation, this));
  zaxis_->SetColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
}

CoordinateObject::~CoordinateObject() {
}

void CoordinateObject::Update(const Camera& camera) {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera.GetProjViewMatrix() * world_);
}

void CoordinateObject::Render(Renderer* renderer) {
  xaxis_->Render(world_, pvw_, renderer);
  yaxis_->Render(world_, pvw_, renderer);
  zaxis_->Render(world_, pvw_, renderer);

  Matrix4 sphere_world = std::move(Scale(0.1f, 0.1f, 0.1f));
  diffuse_effect_->SetDirLight(light_);
  diffuse_effect_->SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
  diffuse_effect_->SetWorld(world_ * sphere_world);
  diffuse_effect_->SetPVW(pvw_ * sphere_world);
  diffuse_effect_->Use(renderer);
  sphere_->Render(renderer);
}
}  // namespace azer
