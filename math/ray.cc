#include "azer/math/ray.h"

#include "azer/math/vector3.h"

namespace azer {
Ray::Ray(const Vector3& pos, const Vector3& dir)
    : position_(pos),
      directional_(dir) {
}
  
Ray::Ray(const Ray& ray) {
  *this = ray;
}

Ray& Ray::operator = (const Ray& ray) {
  this->position_ = ray.position_;
  this->directional_ = ray.directional_;
  return *this;
}
}
