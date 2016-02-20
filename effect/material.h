#pragma once

namespace azer {
struct BaseMaterial {
  TexturePtr diffusemap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;
};

struct TextureMaterial {
  TexturePtr diffusemap;
  TexturePtr emissionmap;
  TexturePtr alphamap;
  TexturePtr normalmap;
};
}  // namespace azer
