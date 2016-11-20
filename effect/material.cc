#include "azer/effect/material.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "azer/base/image.h"
#include "azer/util/images/image.h"
#include "azer/render/blending.h"
#include "azer/render/render_system.h"

namespace azer {

ColorMaterialData::ColorMaterialData() 
    : specular_power(1.0f), 
      alpha(1.0f),
      padding1(0.0f),
      padding2(0.0f) {
}

DiffuseMapMaterialData::DiffuseMapMaterialData() 
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}

TextureMaterialData::TextureMaterialData()
    : ambient_scalar(0.1f),
      specular_scalar(1.0f), 
      alpha(1.0f) {
}
}  // namespace azer
