#include "azer/math/linear.h"

#include "base/logging.h"

namespace azer {
Linear::Linear(float a, float b, float c, float d) {
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
}

Linear::Linear(const Vector3& p1, const Vector3& p2) {
}

Linear::Linear(const Linear& linear) {
  *this = linear;
}

Linear& Linear::operator = (const Linear& l) {
  this->a = l.a;
  this->b = l.b;
  this->c = l.c;
  this->d = l.d;
  return *this;
}

Vector3 Linear::directional() const {
  return Vector3();
}

Vector3 Linear::position() const {
  if (std::abs(a - 0.0f) > 0.000001) {
    return Vector3(-d / a, 0.0f, 0.0f);
  } else if (std::abs(b - 0.0f) > 0.000001) {
    return Vector3(0.0f, -d / b, 0.0f);
  } else if (std::abs(c - 0.0f) > 0.000001) {
    return Vector3(0.0f, 0.0f, -d / c);
  } else {
    CHECK(false);
    return Vector3(0.0f, 0.0f, 0.0f);
  }
}
}  // namespace azer
