#pragma once

#include "azer/render/util/geometry/arrow_object.h"
#include "azer/render/util/geometry/coordinate_object.h"
#include "azer/render/util/geometry/sphere_object.h"

namespace azer {
class CameraSphereObject {
 public:
  CameraSphereObject(const Camera* camera);
  ~CameraSphereObject();

  void Render(Renderer* renderer);

  TransformHolder* GetTransformHolder() { return &holder_;}
  const TransformHolder* GetTransformHolder() const { return &holder_;}
 private:
  void Update();

  const Camera* camera_;
  TransformHolder holder_;
  GeometryObjectPtr sphere_;
  scoped_ptr<AxesFrames> axes_;
  BlendingPtr blending_;

  Matrix4 pvw_;
  Matrix4 world_;
  DirLight light_;
  DISALLOW_COPY_AND_ASSIGN(CameraSphereObject);
};
}  // namespace azer

