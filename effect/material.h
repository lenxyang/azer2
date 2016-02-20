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

class DiffuseMapMaterial : public BaseMaterial {
 public:
  static const char kEffectProviderName[];
  const char* GetProviderName() const override { return kEffectProviderName;}
  DiffuseMapMaterial();
  ~DiffuseMapMaterial() override;

  DiffuseMapMaterialData* mutable_data() { return &data_;}
  const DiffuseMapMaterialData& data() const { return data_;}
  bool Init(const ConfigNode* node, ResourceLoadContext* ctx) override;
  static EffectParamsProvider* CreateObject() { return new DiffuseMapMaterial;}
 private:
  DiffuseMapMaterialData data_;
  DISALLOW_COPY_AND_ASSIGN(DiffuseMapMaterial);
};
}  // namespace azer
