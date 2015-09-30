#pragma once

#include "azer/math/math.h"
#include "azer/render/render.h"
#include "azer/render/util/effects/diffuse_effect.h"
#include "azer/render/util/geometry/geometry_object.h"

namespace azer {
class ArrowObject {
 public:
  ArrowObject();
  explicit ArrowObject(ColoredDiffuseEffectPtr effect);
  ~ArrowObject();

  void SetColor(Vector4 color) { color_ = color;}
  void Render(const Matrix4& world,  const Matrix4& pvw, Renderer* renderer);

  ColoredDiffuseEffectPtr GetEffect() { return effect_;}
  const DirLight& light() const { return light_;}
  DirLight* mutable_light() { return &light_;}
 private:
  void Init(ColoredDiffuseEffectPtr effect);
  Matrix4 cone_world_;
  Matrix4 cylinder_world_;
  Vector4 color_;
  DirLight light_;
  GeometryObjectPtr cone_;
  GeometryObjectPtr cylinder_;
  ColoredDiffuseEffectPtr effect_;
  DISALLOW_COPY_AND_ASSIGN(ArrowObject);
};


}  // namespace azer
