#pragma once

#include "azer/math/math.h"
#include "azer/render/render.h"
#include "azer/render/util/effects/diffuse_effect.h"
#include "azer/render/util/geometry/geometry_object.h"

namespace azer {
class AxesFrames;
class CoordinateAxis {
 public:
  CoordinateAxis(const Matrix4& orientation, AxesFrames* object);
  ~CoordinateAxis();

  void SetColor(const Vector4& color) { color_ = color;}
  void Render(const Matrix4& world,  const Matrix4& pvw, Renderer* renderer);
 private:
  Vector4 color_;
  Matrix4 cone_world_;
  Matrix4 cylinder_world_;
  Matrix4 orientation_;
  AxesFrames* object_; 
  DISALLOW_COPY_AND_ASSIGN(CoordinateAxis);
};

class AxesFrames {
 public:
  AxesFrames();
  ~AxesFrames();

  void Render(const Matrix4& world,  const Matrix4& pvw, Renderer* renderer);
 private:
  GeometryObjectPtr sphere_;
  GeometryObjectPtr cone_;
  GeometryObjectPtr cylinder_;
  ColoredDiffuseEffectPtr diffuse_effect_;

  DirLight light_;
  scoped_ptr<CoordinateAxis> xaxis_;
  scoped_ptr<CoordinateAxis> yaxis_;
  scoped_ptr<CoordinateAxis> zaxis_;

  friend class CoordinateAxis;
  DISALLOW_COPY_AND_ASSIGN(AxesFrames);
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
  scoped_ptr<AxesFrames> axes_;

  Matrix4 pvw_;
  Matrix4 world_;

  DISALLOW_COPY_AND_ASSIGN(CoordinateObject);
};
}  // namespace azer

