#include "azer/math/ray.h"

#include "azer/math/math.h"

namespace azer {
Ray::Ray(const Vector3& pos, const Vector3& dir)
    : origin_(pos),
      directional_(dir) {
}
  
Ray::Ray(const Ray& ray) {
  *this = ray;
}

Ray& Ray::operator = (const Ray& ray) {
  this->origin_ = ray.origin_;
  this->directional_ = ray.directional_;
  return *this;
}
}
