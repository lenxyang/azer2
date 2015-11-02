#pragma once

#include "azer/math/vector3.h"

namespace azer {
class Linear {
 public:
  Linear(float a, float b, float c, float d);
  Linear(const Vector3& p1, const Vector3& p2);
  Linear(const Linear& linear);

  Linear& operator = (const Linear& l);

  Vector3 directional() const;
  Vector3 position() const;
 public:
  float a, b, c, d;
};
}  // namespace azer
