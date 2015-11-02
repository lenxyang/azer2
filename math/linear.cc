#include "azer/math/linear.h"

#include "base/logging.h"

namespace azer {
Linear::Linear(const Vector3& p1, const Vector3& p2) {
}

Linear::Linear(const Linear& linear) {
  *this = linear;
}

Linear& Linear::operator = (const Linear& l) {
  return *this;
}

Vector3 Linear::directional() const {
  return Vector3();
}

Vector3 Linear::position() const {
  return Vector3();
}
}  // namespace azer
