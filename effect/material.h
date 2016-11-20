#pragma once

#include "azer/effect/base_material.h"
#include "azer/math/vector4.h"
#include "azer/render/texture.h"
#include "azer/render/texture_view.h"

namespace azer {
struct ColorMaterialData {
  Vector4  diffuse;
  Vector4  ambient;
  Vector4  specular;
  Vector4  emission;
  float    specular_power;
  float    alpha;
  float    padding1;
  float    padding2;

  ColorMaterialData();
};

struct DiffuseMapMaterialData {
  TextureViewPtr diffusemap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;

  DiffuseMapMaterialData();
};

struct TextureMaterialData {
  TextureViewPtr diffusemap;
  TextureViewPtr specularmap;
  TextureViewPtr emissionmap;
  TextureViewPtr alphamap;
  TextureViewPtr normalmap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;

  TextureMaterialData();
};

}  // namespace azer
