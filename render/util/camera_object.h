#pragma once

#include "azer/render/util/coordinate_object.h"
#include "azer/render/util/arrow_object.h"
#include "azer/render/geometry/sphere_object.h"

namespace azer {
class CameraObject {
 public:
  CameraObject(const Camera* camera);
  ~CameraObject();

  void Update(const Camera& camera);
  void Render(Renderer* renderer);

  void SetSphereColor(const Vector4& color) {  sphere_color_ = color;}
  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  const Camera* camera_;
  TransformHolder holder_;
  Vector4 sphere_color_;
  GeometryObjectPtr sphere_;
  Vector4 frustrum_color_;
  GeometryObjectPtr frustrum_object_;
  scoped_ptr<ArrowObject> arrow_;
  scoped_ptr<AxesFrames> axes_;
  BlendingPtr blending_;

  Matrix4 pvw_;
  Matrix4 world_;
  DirLight light_;
  DISALLOW_COPY_AND_ASSIGN(CameraObject);
};
}  // namespace azer

