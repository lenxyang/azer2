#pragma once

#include "azer/render/render.h"
#include "azer/render/util.h"

namespace azer {
class CoordinateObject;
class CoordinateAxis {
 public:
  CoordinateAxis(const Matrix4& orientation, CoordinateObject* object);
  ~CoordinateAxis();

  void SetColor(const Vector4& color) { color_ = color;}
  void Render(const Matrix4& world,  const Matrix4& pvw, Renderer* renderer);
 private:
  Vector4 color_;
  Matrix4 cone_world_;
  Matrix4 cylinder_world_;
  Matrix4 orientation_;
  CoordinateObject* object_; 
  DISALLOW_COPY_AND_ASSIGN(CoordinateAxis);
};


class CoordinateObject {
 public:
  CoordinateObject();
  ~CoordinateObject();

  void Update(const Camera& camera);
  void Render(Renderer* renderer);

  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  TransformHolder holder_;
  GeometryObjectPtr sphere_;
  GeometryObjectPtr cone_;
  GeometryObjectPtr cylinder_;
  ColoredDiffuseEffectPtr diffuse_effect_;

  Matrix4 pvw_;
  Matrix4 world_;
  DirLight light_;
  scoped_ptr<CoordinateAxis> xaxis_;
  scoped_ptr<CoordinateAxis> yaxis_;
  scoped_ptr<CoordinateAxis> zaxis_;

  friend class CoordinateAxis;
  DISALLOW_COPY_AND_ASSIGN(CoordinateObject);
};
}  // namespace azer
