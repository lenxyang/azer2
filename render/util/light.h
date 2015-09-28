#pragma once

#include "azer/math/math.h"

namespace azer {
struct DirLight {
  Vector4 dir;
  Vector4 diffuse;
  Vector4 ambient;
};

}  // namespace azer
