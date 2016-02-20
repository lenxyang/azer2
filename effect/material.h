#pragma once

#include "azer/effect/base_material.h"
#include "azer/math/vector4.h"
#include "azer/render/texture.h"

namespace azer {
struct ColorMaterialData {
  Vector4 diffusemap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;
};

struct DiffuseMapMaterialData {
  TexturePtr diffusemap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;

  DiffuseMapMaterialData();
};

struct TextureMaterialData {
  TexturePtr diffusemap;
  TexturePtr emissionmap;
  TexturePtr alphamap;
  TexturePtr normalmap;
};
}  // namespace azer
