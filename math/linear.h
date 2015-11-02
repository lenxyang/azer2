#pragma once

#include "azer/math/vector3.h"
#include "azer/math/vector4.h"

namespace azer {
class Linear {
 public:
  Linear(const Vector3& p1, const Vector3& p2);
  Linear(const Linear& linear);

  Linear& operator = (const Linear& l);

  Vector3 directional() const;
  Vector3 position() const;
 public:
  Vector4 v1_;
  Vector4 v2_;
};
}  // namespace azer
