#pragma once

#include "azer/math/vector3.h"

namespace azer {
class Ray {
 public:
  Ray();
  Ray(const Ray& ray);
  Ray(const Vector3& pos, const Vector3& dir);
  Ray& operator = (const Ray& ray);

  const Vector3& directional() const { return directional_;}
  const Vector3& origin() const { return origin_;}
 private:
  Vector3 origin_;
  Vector3 directional_;
};
}  // namespace azer
