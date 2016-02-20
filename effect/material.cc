#include "azer/effect/material.h"

namespace azer {
DiffuseMapMaterialData::DiffuseMapMaterialData() 
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}
}  // namespace azer
