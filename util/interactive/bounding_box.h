#pragma once

#include "base/memory/ref_counted.h"
#include "azer/math/vector3.h"
#include "azer/math/vector4.h"
#include "azer/math/matrix4.h"

namespace azer {

class Camera;
class ColorEffect;
class Entity;
class Renderer;

class BoundingBoxObj {
 public:
  BoundingBoxObj();
  
  void SetColor(const Vector4& color) { color_ = color;}
  void SetCorner(const Vector3& min, const Vector3& max);
  void Update(const Camera* camera);
  void Render(Renderer* renderer);
 private:
  Vector3 min_, max_;
  Vector4 color_;
  Matrix4 world_;
  scoped_refptr<Entity> box_;
  scoped_refptr<Entity> boxframe_;
  scoped_refptr<ColorEffect> color_effect_;
  DISALLOW_COPY_AND_ASSIGN(BoundingBoxObj);
};
}  // namespace azer
