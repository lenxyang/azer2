#include "azer/math/ray.h"

#include "azer/math/vector3.h"

namespace azer {
Ray::Ray(const Vector3& pos, const Vector3& dir)
    : directional_(dir) {
  d_ = -(dir.x * pos.x + dir.y * pos.y + dir.z * pos.z);
  if (std::abs(dir.x - 0.0f) > 0.000001) {
    position_ = Vector3(-d_ / dir.x, 0.0f, 0.0f);
  } else if (std::abs(dir.y - 0.0f) > 0.000001) {
    position_ = Vector3(0.0f, -d_ / dir.y, 0.0f);
  } else if (std::abs(dir.z - 0.0f) > 0.000001) {
    position_ = Vector3(0.0f, 0.0f, -d_ / dir.z);
  } else {
    position_ = Vector3(0.0f, 0.0f, 0.0f);
  }
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
