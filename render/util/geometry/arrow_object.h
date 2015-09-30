#pragma once

#include "azer/math/math.h"
#include "azer/render/render.h"
#include "azer/render/util/effects/diffuse_effect.h"
#include "azer/render/util/geometry/geometry_object.h"

namespace azer {
class ArrowObject {
 public:
  ArrowObject();
  ~ArrowObject();

  void SetColor(const Vector4& color) { color_ = color;}
  TransformHolder* GetTransformHolder() { return &holder_;}
  void Update(const Camera& camera);
  void Render(Renderer* renderer);
 private:
  Vector4 color_;
  DirLight light_;
  Matrix4 cone_world_;
  Matrix4 cylinder_world_;
  Matrix4 world_;
  Matrix4 pvw_;
  TransformHolder holder_;

  GeometryObjectPtr cone_;
  GeometryObjectPtr cylinder_;
  ColoredDiffuseEffectPtr diffuse_effect_;
  DISALLOW_COPY_AND_ASSIGN(ArrowObject);
};

}  // namespace azer
